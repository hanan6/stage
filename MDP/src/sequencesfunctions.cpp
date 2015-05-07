#include "sequencesfunctions.h"
#include "alignement.h"
#include <algorithm>
#include <iterator>

using namespace std;

namespace mdp {

vector<string> support(vector<string> const &sequences, vector<Pattern> const &patterns) {
    vector<string> ret;

    for(auto const &seq: sequences) {
        bool supported = true;
        #pragma omp parallel for schedule(dynamic,1) reduction(&:supported)
        for(size_t i = 0; i < patterns.size(); ++i) {
            supported &= patterns[i].search(seq);
        }
        if(supported)
            ret.push_back(seq);
    }

    return ret;
}

matrix<bool> computeSupportMatrix(vector<string> const& sequences, vector<Pattern> const &patterns) {
    matrix<bool> ret(sequences.size(), patterns.size(), false);

    for(size_t s = 0; s < sequences.size(); ++s) {
        //#pragma omp parallel for schedule(dynamic, 1) can't because matrix of bool !!
        for(size_t p = 0; p < patterns.size(); ++p) {
            ret(s,p) = patterns[p].search(sequences[s]);
        }
    }

    return ret;
}

matrix<vector<size_t>> computeEmbeddingsMatrix(vector<string> const& sequences, vector<Pattern> const &patterns) {
    matrix<vector<size_t>> ret(sequences.size(), patterns.size(), vector<size_t>());

    for(size_t s = 0; s < sequences.size(); ++s) {
        #pragma omp parallel for schedule(dynamic, 1)
        for(size_t p = 0; p < patterns.size(); ++p) {
            size_t pos{};
            while((pos = patterns[p].search(sequences[s], pos)) != string::npos)
                ret(s,p).push_back(pos++);
        }
    }

    return ret;
}


// throw doesn't cover..
matrix<size_t> computeFirstEmbeddingsMatrix(vector<string> const& sequences, vector<Pattern> const &patterns) {
    matrix<size_t> ret(sequences.size(), patterns.size());

    for(size_t s = 0; s < sequences.size(); ++s) {
        #pragma omp parallel for schedule(dynamic, 1)
        for(size_t p = 0; p < patterns.size(); ++p) {
            size_t pos{};
            ret(s,p) = patterns[p].search(sequences[s], pos);
        }
    }

    return ret;
}

vector<Pattern> decomposePatternsSlow(vector<Pattern>& patterns, const size_t slack) {

    // assume patterns is sorted
    vector<Pattern> newPatterns;
    auto it = patterns.begin();
    for(; it != patterns.end(); ++it) {
        if(it->slack() <= slack) {
         break;
        } else {
            size_t count     = 0;
            size_t maxCount     = 0;
            auto start     = 0;
            auto lastSolid = 0;
            //cout << p << endl;
            string p = it->toString();
            for(size_t i = 0; i < p.size(); ++i) {
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
    vector<Pattern> ret;
    //ret.reserve(distance(it,patterns.end()) + newPatterns.size());
    merge
        ( make_move_iterator(it), make_move_iterator(patterns.end())
        , make_move_iterator(newPatterns.begin()), make_move_iterator(newPatterns.end())
        , back_inserter(ret)
        , Pattern::grSlackTotal()
        );

    return removeIncluded(ret);;
}


}
