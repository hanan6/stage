#include "alignement.h"
#include "utility.h"
#include "sequencesfunctions.h"
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <numeric>
#include <iterator> // test


using namespace std;

namespace mdp {

Pattern subPattern
    ( string const & s, Pattern const & p
    , size_t begin_s
    , Pattern::iterator begin_p, Pattern::iterator end_p
    )
{
    string pat{};
    Pattern::iterator cp_begin = begin_p;
    while(begin_p != end_p) {
        if(begin_p != cp_begin && begin_p->second > 0) {
            pat.append(begin_p->second, '.');
            begin_s += begin_p->second;
        }
        pat += (begin_p->first == s[begin_s]) ? begin_p->first : '.';
        ++begin_s;
        ++begin_p;
    }
    if(begin_p == end_p) {
        if(begin_p != cp_begin && begin_p->second > 0) {
            pat.append(begin_p->second, '.');
            begin_s += begin_p->second;
        }
        pat += (begin_p->first == s[begin_s]) ? begin_p->first : '.';
    }

    return Pattern(pat);
}

Pattern subPattern
    ( string const &s1, string const &s2
    , size_t begin1, size_t end1
    , size_t begin2
    )
{
    string pat{};
    while(begin1 <= end1) {
        pat += (s1[begin1] == s2[begin2]) ? s1[begin1] : '.';
        begin1++;begin2++;
    }

    return Pattern(pat);
}



std::vector<Pattern> searchCommonPatternsImpl
    ( std::string const &s1
    , std::string const &s2
    , size_t             maxslack
    , size_t             sizeMin
    );

std::vector<Pattern> searchCommonPatterns
    ( std::string const &s1
    , std::string const &s2
    , size_t             maxslack
    , size_t             sizeMin
    )
{
    if(s1.size() < s2.size())
        return searchCommonPatternsImpl(s1, s2, maxslack, sizeMin);
    else
        return searchCommonPatternsImpl(s2, s1, maxslack, sizeMin);
}


std::vector<Pattern> searchCommonPatternsImpl
    ( std::string const &s1
    , std::string const &s2
    , size_t             maxslack
    , size_t             sizeMin
    )
{
    size_t num_threads = omp_get_max_threads();
    vector<vector<Pattern>> patterns{num_threads};

    int tid{};

    // [ s2     ]
    // [ s1 ]
    // to
    // [ s2     ]
    //        [ s1 ]
    #pragma omp parallel for schedule(dynamic,1) private(tid)
    for(size_t i = 0; i < s2.size() - 1; ++i) {
        tid = omp_get_thread_num();
        size_t begin1 = 0; // position that holds where the matching start in s1
        size_t end1   = 0; // position that holds where is the matching currently in s1
        size_t begin2 = 0; // position that holds where the matching start in s1
        size_t count  = 0; // count the number of solids caracters found during the current matching
        size_t slack  = 0; // number of consecutive mismatch
        for(size_t j = 0; j < min(s1.size(), s2.size() - i); ++j) {
            // if there is a match update the information about the begining and the ending
            // of the match
            if(s1[j] == s2[i+j] && s1[j] != '.') {
                if(count == 0) {
                    begin1 = j;
                    begin2 = i+j;
                }
                count++;
                end1=j;
                slack = 0;
            } else {
                // increment slack only if there is a match before
                if(count != 0)
                    slack++;
                // if the slack is greater than maxSlack and if the number of solids
                // caracters is greater or equal than size we found a potential pattern.
                if(slack > maxslack) {
                    if(count >= sizeMin) {
                        patterns[tid].push_back(subPattern(s1,s2,begin1,end1,begin2));
                    }
                    slack = 0;
                    count = 0;
                }
            }
        }
        if(count >= sizeMin) {
            patterns[tid].push_back(subPattern(s1,s2,begin1,end1,begin2));
        }
    }


    //     [ s2     ]
    //    [ s1 ]
    // to
    //     [ s2     ]
    // [ s1 ]
    #pragma omp parallel for schedule(dynamic,1) private(tid)
    for(size_t j = 1; j < s1.size() -1; ++j) {
        tid = omp_get_thread_num();
        size_t begin1 = 0;
        size_t end1   = 0;
        size_t begin2 = 0;
        size_t count  = 0;
        size_t slack  = 0;
        size_t i = 0;
        while(j+i < s1.size()) {
            if(s1[i+j] == s2[i]&& s1[i+j] != '.') {
                if(count == 0) {
                    begin1 = i+j;
                    begin2 = i;
                }
                count++;
                end1=i+j;
                slack = 0;
            } else {
                if(count != 0)
                    slack++;
                if(slack > maxslack) {
                    if(count >= sizeMin) {
                        patterns[tid].push_back(subPattern(s1,s2,begin1,end1,begin2));
                    }
                    slack = 0;
                    count = 0;
                }
            }
            ++i;
        }
        if(count >= sizeMin) {
            patterns[tid].push_back(subPattern(s1,s2,begin1,end1,begin2));
        }
    }

    sortAndMegreVector(patterns, Pattern::grSizeTotal());
    removeDuplicates(patterns.front());

    return patterns.front();
}

vector<Pattern> removeIncluded(const vector<Pattern>& patterns) {
    enum {CHUNK = sizeof(vector<bool>::size_type)};
    vector<Pattern> maxPats;
    vector<int> oks{}; // vector<bool> isn't thread safe
    oks.resize(patterns.size());
    size_t i = 0;
    //#pragma omp parallel for schedule(static, CHUNK) private(i)
    #pragma omp parallel for schedule(dynamic, 1) private(i)
    for( i = 0; i < patterns.size(); ++i) {
        bool isIncl = false;
        for(size_t j = 0; j < patterns.size() && !isIncl; ++j) {
            if(i != j) {
                isIncl = patterns[i].search(patterns[j]);
            }
        }
        oks[i] = !isIncl;
    }

    for(size_t i = 0; i < patterns.size(); ++i) {
        if(oks[i])
            maxPats.push_back(patterns[i]);
    }

    return maxPats;
}



vector<Pattern> searchCommonPatterns
    ( string  const &s
    , Pattern const &p
    , size_t         maxslack
    , size_t         sizeMin
    )
{
    size_t num_threads = omp_get_max_threads();
    vector<vector<Pattern>> patterns{num_threads};

    int tid{};
    auto const pat_end = p.end() ;

    #pragma omp parallel private(tid)
    {
        //     [ s2     ]
        //    [ p  ]
        // to
        //     [ s2     ]
        // [ p  ]
        #pragma omp single
        {
            tid = omp_get_thread_num();
            size_t     adv     = 0;
            auto       it      = p.begin() ;
            it++;
            adv = it->second + 1;
            while(it != pat_end) {
                auto it_cur   = it;
                auto it_begin = it_cur ;
                auto it_end   = it_cur ;
                size_t i        = adv-1;
                size_t begin_s  = 0 ;
                size_t count    = 0 ;
                size_t slack    = 0 ;
                while(it_cur != pat_end && i < s.size()) {
                    if(it_cur->first == s[i]) {
                        if(count == 0) {
                            it_begin = it_cur;
                            begin_s = i;
                        }
                        count++;
                        it_end = it_cur;
                        slack = 0;
                    } else {
                        if(count != 0) {
                            slack++;
                        }
                        if(slack > maxslack) {
                            if(count >= sizeMin) {
                                patterns[tid].push_back(subPattern(s,p,begin_s, it_begin, it_end));
                            }
                            slack = 0;
                            count = 0;
                        }
                    }
                    ++it_cur;
                    auto off = it_cur->second + 1;
                    slack   += it_cur->second;
                    if(slack > maxslack) {
                        if(count >= sizeMin) {
                            patterns[tid].push_back(subPattern(s,p,begin_s, it_begin, it_end));
                        }
                        slack = 0;
                        count = 0;
                    }
                    i += off;
                }
                if(count >= sizeMin) {
                    patterns[tid].push_back(subPattern(s,p,begin_s, it_begin, it_end));
                }
                if(adv == 1) {
                    ++it;
                    adv = it->second + 1;
                } else {
                    --adv;
                }
            }
        }

        // [ s2     ]
        // [ p  ]
        // to
        // [ s2     ]
        //        [ p  ]
        #pragma omp for schedule(dynamic,1)
        for(size_t i = 0; i < s.size() - 1; ++i) {
            tid = omp_get_thread_num();
            auto   it_beg   = p.begin() ;
            auto   it_end   = p.begin() ;
            auto   it_cur   = p.begin() ;
            size_t begin_s  = 0 ;
            size_t count    = 0 ;
            size_t slack    = 0 ;
            size_t adv      = 0 ;

            while(it_cur != pat_end && adv + i < s.size()) {
                if(s[i+adv] == it_cur->first) {
                    if(count == 0) {
                        it_beg   = it_cur;
                        begin_s  = i+adv;
                    }
                    count++;
                    it_end = it_cur;
                    slack = 0;
                } else {
                    if(count != 0)
                        slack++;
                    if(slack > maxslack) {
                        if(count >= sizeMin) {
                            patterns[tid].push_back(subPattern(s, p, begin_s, it_beg, it_end));
                        }
                        slack = 0;
                        count = 0;
                    }
                }
                ++it_cur;
                auto off = it_cur->second + 1;
                slack   += it_cur->second;
                if(slack > maxslack) {
                    if(count >= sizeMin) {
                        patterns[tid].push_back(subPattern(s, p, begin_s, it_beg, it_end));
                    }
                    slack = 0;
                    count = 0;
                }
                adv += off;
            }
            if(count >= sizeMin) {
                patterns[tid].push_back(subPattern(s, p, begin_s, it_beg, it_end));
            }
        }
    }


    sortAndMegreVector(patterns, Pattern::grSizeTotal());
    removeDuplicates(patterns.front());

    return patterns.front();
}

vector<Pattern> searchMaxPatternsSlack(vector<string> const &sequences, size_t slack) {
    return searchMaxPatternsSlack(sequences.begin(), sequences.end(), slack);
}

pair<vector<Pattern>, vector<Pattern>>
searchIncludedAndNotPatterns(string const &s, vector<Pattern>& patterns) {
    vector<Pattern> common{};
    vector<Pattern> different{};

    for(const auto& p: patterns) {
        if(p.search(s)) {
            common.push_back(move(p));
        } else if(p.solids() > 2) {
            different.push_back(move(p));
        }
    }

    return make_pair(move(common), move(different));
}

bool check2Blocks(string const &s, vector<string> const&v, size_t slack) {
    for(size_t i = 0; i < s.size() - (1 + slack); ++i) {
        auto c1 = s[i];
        auto c2 = s[i+1+slack];
        bool common = true;
        for(const auto& str: v) {
            bool pres = false;
            for(size_t j = 0; j < str.size() - (1 + slack) && !pres; ++j) {
                auto c1_ = str[j];
                auto c2_ = str[j+1+slack];
                pres = (c1 == c1_) && (c2 == c2_);
            }
            common &= pres;
            if(!common) break;
        }
        if(common) return true;
    }
    return false;
}


#if 0
vector<Pattern> decomposePatterns(vector<Pattern>& patterns, const int slack) {
    // assume patterns is sorted
    vector<Pattern> newPatterns;
    auto it = patterns.begin();
    for(; it != patterns.end(); ++it) {
        if(it->slack() <= slack) {
         break;
        } else {
            auto count     = 0;
            auto maxCount     = 0;
            auto start     = 0;
            auto lastSolid = 0;
            //cout << p << endl;
            string p = it->toString();
            for(auto i = 0; i < p.size(); ++i) {
                if(p[i] == '.') {
                    ++count;
                } else {
                    maxCount = max(count, maxCount);
                    if(count > slack) {
                        if(start!= lastSolid) {
                            auto pat = p.substr(start, lastSolid-start+1);
                            newPatterns.emplace_back(pat);
                        }
                        start = i;
                        maxCount = 0;
                    }
                    lastSolid = i;
                    count = 0;
                }
            }
            if(maxCount <= slack && lastSolid != start ) {
                auto pat = p.substr(start, lastSolid-start+1);
                newPatterns.emplace_back(pat);
            }
        }
    }
    removeDuplicates(newPatterns);
    removeIncluded(newPatterns);
    vector<Pattern> ret;
    //ret.reserve(distance(it,patterns.end()) + newPatterns.size());
    merge
        ( make_move_iterator(it), make_move_iterator(patterns.end())
        , make_move_iterator(newPatterns.begin()), make_move_iterator(newPatterns.end())
        , back_inserter(ret)
        );

    return ret;
}
#endif
}
