#include "mdp.h"
#include "alignement.h"
#include "sequencesfunctions.h"
#include "setcoveringoptions.h"
#include "setcovering.h"
#include <algorithm>
#include <memory>
#include <gecode/search.hh> // BAB

#include <time.h>
#include <chrono>

#include <map>

#include "xmlwritter.h"
#include "tracedebug.h"

#include "zip_iterator.h"
#include "sortpermuteiter.h"
#include "sequencesfunctions.h"
/*#include <boost/range.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm/for_each.hpp>

template <typename... T>
auto zip(T&... containers)
    -> boost::iterator_range<decltype(iterators::makeTupleIterator(std::begin(containers)...))> {
  return boost::make_iterator_range(iterators::makeTupleIterator(std::begin(containers)...),
                                    iterators::makeTupleIterator(std::end(containers)...));
}*/




using namespace std;
using namespace Gecode;

namespace mdp {
MDP::MDP(std::shared_ptr<database::Database> db, size_t minSlack, size_t maxSlack)
    :db(db), m_minSlack(minSlack), m_maxSlack(maxSlack)
{
}

MDP::~MDP() {
}

void MDP::run(string const &outpuFile) {
    XMLWritter xmlw;
    auto td = getTraceInstance();
    for(string const & name: *db) {
        cout << setfill('>') << setw(5) << ""
             << " " << "Class : " << name
             << " " << setfill('<') << setw(5) << ""
             << setfill(' ')
             << endl;
        if(TraceDebug::patternRequired) {
            ostringstream os;
            os << "times_" << name << ".csv";
            td->setTimes(os.str());
        }
        vector<string> sequences;
        vector<string> sequencesNames;
        vector<string> negSequences;
        db->getBase(name, sequences);
        db->getNames(name, sequencesNames);
        db->getBasesExcept(name, negSequences);
        if(sequences.empty()) {
            return;
        } else if(sequences.size() == 1) {
            return;
        }
        // sort acording to string size

        sortByPerm(sequences, sequencesNames);

        vector<Pattern> bestPatterns{}; // set of patterns
        vector<bool>    bestSelected{}; // set of selected patterns
        size_t          bestExcl{};    //
        size_t          bestSlack{};   //
        size_t          bestCard{};   // not used yet, but could be useful for some optimization on the COP
        size_t          sMax = getSlacks(sequences).back();
        for(size_t slack : getSlacks(sequences)) {
            //cout << "\t" << slack << "/" << sMax << endl;
           /* cout << setfill('>') << setw(5) << ""
                 << " " << "Class : " << name
                 << " " << setfill('<') << setw(5) << ""
                 << setfill(' ')
                 << " : " << slack << "/" << sMax << endl;*/


            if(TraceDebug::timesRequied) {
                ostringstream os;
                os << "patterns_" << name << "_" << slack << ".csv";
                td->setPatterns(os.str());
            }

            auto startChrono = std::chrono::high_resolution_clock::now();
            /*if(TraceDebug::timesRequied) {
                startChrono =  std::chrono::high_resolution_clock::now();
            }*/

            //cout << "\tComputing maximal patterns..." << endl;
            auto patterns   = searchMaxPatternsSlack(sequences, slack);
            if(patterns.empty()) continue;

            auto supportMat = computeSupportMatrix(negSequences, patterns);

           // cout << "\tComputing maximal motif (" << patterns.size() << " patterns found)..." << endl;
            auto endPhase1 = std::chrono::high_resolution_clock::now();
            SetCoveringOptions sco(&supportMat, bestExcl);
            SetCovering* sc = new SetCovering(sco);
/*
            if(name == "9") {
            Gist::Print<SetCovering> p("Print solution");
            Gist::Options o;
            o.inspect.click(&p);
            Gist::bab(sc, o);
            }*/

            BAB<SetCovering> e = BAB<SetCovering>(sc);//BAB
            delete sc;

            SetCovering *bestSol = 0;

            // BaB
            while (SetCovering* s = e.next()) {
                if(bestSol != 0)
                    delete bestSol;
                 bestSol = s;
            }
            /*
            if(name == "9") {
                cout << slack << endl;
                copy(patterns.begin(), patterns.end(), ostream_iterator<Pattern>(cout, "\n"));
            }
            */
            if(TraceDebug::timesRequied) {
                auto endPhase2 = std::chrono::high_resolution_clock::now();
                auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(endPhase1 - startChrono).count();
                auto time2 = std::chrono::duration_cast<std::chrono::milliseconds>(endPhase2 - endPhase1).count();
                td->writeTime(slack, time1, time2);
            }
            if(bestSol != 0 && bestExcl < (size_t)bestSol->getExclusion()) {
                bestExcl     = bestSol->getExclusion();
               // cout << "\tExclusion : " << bestExcl << "/" << negSequences.size() << endl;
                bestSlack    = slack;
                bestCard     = bestSol->getCard();
                bestPatterns = move(patterns);
                bestSelected = bestSol->getSelectedPatterns();
                delete bestSol;
                if(bestExcl == negSequences.size()) break;
            }
            //cout << "\t\t" << bestExcl << endl;
        }

        // a solution was found

        typedef std::shared_ptr<std::vector<Pattern>> ptr_vectorP;
        typedef std::shared_ptr<std::string>          ptr_vectorS;
        typedef std::shared_ptr<matrix<size_t>>       ptr_matrixS;
        if(bestPatterns.size() > 0) {
            auto names    = make_shared<vector<string>>(move(sequencesNames));
            auto patterns = make_shared<vector<Pattern>>(vector<Pattern>());

            auto beg = make_zip_iterator(bestPatterns.begin(), bestSelected.begin());
            auto end = make_zip_iterator(bestPatterns.end(), bestSelected.end());
            for(auto it = beg; it != end; ++it) {
                if(*(it.second))
                    patterns->push_back(move(*(it.first)));
            }
            auto embeddings = make_shared<matrix<size_t>>(computeFirstEmbeddingsMatrix(sequences, *patterns));
            auto mmPos = minmax_element
                    ( sequences.begin(), sequences.end()
                    , [](string const &lhs, string const &rhs) {
                        return lhs.size() < rhs.size();
                      }
                    );
            auto mmNeg = minmax_element
                    ( negSequences.begin(), negSequences.end()
                    , [](string const &lhs, string const &rhs) {
                        return lhs.size() < rhs.size();
                      }
                    );
            size_t minPos = (mmPos.first)->size(),
                   maxPos = (mmPos.second)->size(),
                   minNeg = (mmNeg.first)->size(),
                   maxNeg = (mmNeg.second)->size();
            xmlw.addEntry
                (
                    XMLWritter::Entry
                    ( name
                    , sequences.size(), negSequences.size()
                    , minPos, maxPos
                    , minNeg, maxNeg
                    , names
                    , patterns->size(), negSequences.size() - bestExcl, bestSlack
                    , patterns
                    , embeddings
                    )
                );

        } else {
            auto mmPos = minmax_element
                    ( sequences.begin(), sequences.end()
                    , [](string const &lhs, string const &rhs) {
                        return lhs.size() < rhs.size();
                      }
                    );
            auto mmNeg = minmax_element
                    ( negSequences.begin(), negSequences.end()
                    , [](string const &lhs, string const &rhs) {
                        return lhs.size() < rhs.size();
                      }
                    );
            size_t minPos = (mmPos.first)->size(),
                   maxPos = (mmPos.second)->size(),
                   minNeg = (mmNeg.first)->size(),
                   maxNeg = (mmNeg.second)->size();
            xmlw.addEntry
                (
                    XMLWritter::Entry
                    ( name
                    , sequences.size(), negSequences.size()
                    , minPos, maxPos
                    , minNeg, maxNeg
                    )
                );

        }
    }
    xmlw.write(outpuFile);
}

void MDP::runEmbeddings(string const &outpuFile) {

    for(string const & name: *db) {
        cout << setfill('>') << setw(5) << ""
             << " " << "Class : " << name
             << " " << setfill('<') << setw(5) << ""
             << setfill(' ')
             << endl;
        vector<string> sequences;
        vector<string> sequencesNames;
        vector<string> negSequences;
        db->getBase(name, sequences);
        db->getNames(name, sequencesNames);
        db->getBasesExcept(name, negSequences);
        if(sequences.empty()) {
            return;
        } else if(sequences.size() == 1) {
            return;
        }
        // sort acording to string size

        sortByPerm(sequences, sequencesNames);


        vector<size_t> slacks = getSlacks(sequences);
        if(slacks.empty()) continue;
        size_t slack = slacks.back();

        auto patterns   = searchMaxPatternsSlack(sequences, slack);
        if(patterns.empty()) continue;


        sort(patterns.begin(), patterns.end(), Pattern::grSlackTotal());
        for(auto it = slacks.rbegin(); it != slacks.rend(); ++it) {
            if(slack != *it) {
                patterns = decomposePatternsSlow(patterns, *it);
            }
            auto embMatPos = computeEmbeddingsMatrix(sequences, patterns);
            auto embMatNeg = computeEmbeddingsMatrix(negSequences, patterns);
            ostringstream os;
            os << outpuFile << "_" << name << "_" << *it << ".txt";
            cout << os.str() << endl;
            ofstream file(os.str());
            file << patterns.size() << endl;
            for(Pattern const &p: patterns) {
                file << p << endl;
            }
            for(Pattern const &p: patterns) {
                file << p.size() <<  " ";
            }
            file << endl;
            for(Pattern const &p: patterns) {
                file << p.slack() <<  " ";
            }
            file << endl;
            for(Pattern const &p: patterns) {
                file << p.solids() <<  " ";
            }
            file << endl;
            for(size_t s = 0; s < sequences.size(); ++s) {
                for(size_t p = 0; p < patterns.size(); ++p) {
                    file << embMatPos(s,p).size() << " ";
                    copy(embMatPos(s,p).begin(), embMatPos(s,p).end(), ostream_iterator<size_t>(file, " "));
                    file << endl;
                }
            }
            for(size_t s = 0; s < negSequences.size(); ++s) {
                for(size_t p = 0; p < patterns.size(); ++p) {
                    file << embMatNeg(s,p).size() << " ";
                    copy(embMatNeg(s,p).begin(), embMatNeg(s,p).end(), ostream_iterator<size_t>(file, " "));
                    file << endl;
                }
            }
        }
    }
}

void MDP::runPatterns(string const &outpuFile) {

    for(string const & name: *db) {
        cout << setfill('>') << setw(5) << ""
             << " " << "Class : " << name
             << " " << setfill('<') << setw(5) << ""
             << setfill(' ')
             << endl;
        vector<string> sequences;
        vector<string> sequencesNames;
        vector<string> negSequences;
        db->getBase(name, sequences);
        db->getNames(name, sequencesNames);
        db->getBasesExcept(name, negSequences);
        if(sequences.empty()) {
            return;
        } else if(sequences.size() == 1) {
            return;
        }
        // sort acording to string size

        sortByPerm(sequences, sequencesNames);


        vector<size_t> slacks = getSlacks(sequences);
        if(slacks.empty()) continue;
        size_t slack = slacks.back();

        auto patterns   = searchMaxPatternsSlack(sequences, slack);
        if(patterns.empty()) continue;


        sort(patterns.begin(), patterns.end(), Pattern::grSlackTotal());

        ostringstream os;
        os << outpuFile << "_" << name << ".txt";
        cout << os.str() << endl;
        ofstream file(os.str());
        map<size_t, size_t> slack_patterns;
        for(auto it = slacks.rbegin(); it != slacks.rend(); ++it) {
            if(slack != *it) {
                patterns = decomposePatternsSlow(patterns, *it);
            }
            slack_patterns[*it] = patterns.size();
        }
        for(auto v: slack_patterns) {
            file << v.first << "\t" << v.second << endl;
        }
    }
}




void MDP::runPatternsSeq(string const &outpuFile) {
    auto td = getTraceInstance();
    TraceDebug::patternRequired = true;
    for(string const & name: *db) {
        cout << setfill('>') << setw(5) << ""
             << " " << "Class : " << name
             << " " << setfill('<') << setw(5) << ""
             << setfill(' ')
             << endl;
        vector<string> sequences;
        vector<string> sequencesNames;
        vector<string> negSequences;
        db->getBase(name, sequences);
        db->getNames(name, sequencesNames);
        db->getBasesExcept(name, negSequences);
        if(sequences.empty()) {
            return;
        } else if(sequences.size() == 1) {
            return;
        }
        // sort acording to string size

        sortByPerm(sequences, sequencesNames);

        for(size_t slack : getSlacks(sequences)) {


            if(TraceDebug::patternRequired) {
                ostringstream os;
                os << outpuFile << "_" << name << "_" << slack << ".csv";
                td->setPatterns(os.str());
                cout << os.str() << endl;
            }

            searchMaxPatternsSlack(sequences, slack);
        }
    }
}

void MDP::runPatternsTime(string const &outpuFile) {
    for(string const & name: *db) {
        cout << setfill('>') << setw(5) << ""
             << " " << "Class : " << name
             << " " << setfill('<') << setw(5) << ""
             << setfill(' ')
             << endl;
        vector<string> sequences;
        vector<string> sequencesNames;
        vector<string> negSequences;
        db->getBase(name, sequences);
        db->getNames(name, sequencesNames);
        db->getBasesExcept(name, negSequences);
        if(sequences.empty()) {
            return;
        } else if(sequences.size() == 1) {
            return;
        }
        // sort acording to string size

        sortByPerm(sequences, sequencesNames);


        ostringstream os;
        os << outpuFile << "_" << name << ".csv";
        ofstream file(os.str());
        cout << os.str() << endl;

        map<size_t, size_t> slack_times;
        for(size_t slack : getSlacks(sequences)) {
            auto startChrono = std::chrono::high_resolution_clock::now();
            searchMaxPatternsSlack(sequences, slack);
            auto endChrono = std::chrono::high_resolution_clock::now();
            auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(endChrono - startChrono).count();
            slack_times[slack] = time1;
        }

        for(auto v: slack_times) {
            file << v.first << " " << v.second << endl;
        }
    }
}

vector<size_t> MDP::getSlacks(vector<string>const &sequences) {
    vector<size_t> slacks{};
    for(size_t i = m_minSlack; i <= min(sequences.front().size() - 2, m_maxSlack); ++i) {
        if(check2Blocks(sequences.front(), sequences, i))
            slacks.push_back(i);
    }
    return slacks;
}
}
