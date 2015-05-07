#ifndef ALIGNEMENT_H
#define ALIGNEMENT_H

#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <iterator>
#include <chrono>
#include <time.h>
#include "omp.h"
#include "pattern.h"
#include "tracedebug.h"



namespace mdp {

//!
//! \brief subPattern
//! \param s string
//! \param p pattern
//! \param begin_s indice start string
//! \param begin_p iterator start pattern
//! \param end_p   iterator end pattern
//! \return
//!
Pattern subPattern
    ( std::string const & s, Pattern const & p
    , size_t begin_s
    , Pattern::iterator begin_p, Pattern::iterator end_p
    );

//!
//! \brief subPattern
//! \param s1
//! \param s2
//! \param begin1
//! \param end1
//! \param begin2
//! \return
//!
Pattern subPattern
    ( std::string const &s1, std::string const &s2
    , size_t begin1, size_t end1
    , size_t begin2
    );



//!
//! \brief computeCommonPatterns
//! \param s1 The first sequence
//! \param s2 The second sequence
//! \param maxslack The slack maximum of the patterns
//! \param sizeMin The minimal size of the patterns
//! \return
//!
std::vector<Pattern> searchCommonPatterns
    ( std::string const &s1
    , std::string const &s2
    , size_t             maxslack = 0
    , size_t             sizeMin  = 2
    );

//!
//! \brief removeIncluded
//! \param patterns
//! \return
//!
std::vector<Pattern> removeIncluded(std::vector<Pattern> const& patterns);


//!
//! \brief searchCommonPatterns
//! \param s
//! \param p
//! \param maxslack
//! \param sizeMin
//! \return
//!
std::vector<Pattern> searchCommonPatterns
    ( std::string  const &s
    , Pattern      const &p
    , size_t              maxslack = 0
    , size_t              sizeMin  = 2
    );


//!
//! \brief searchCommonAndDiffernt
//! \param s
//! \param patterns
//! \return
//!
std::pair<std::vector<Pattern>, std::vector<Pattern>>
searchIncludedAndNotPatterns(std::string const &s, std::vector<Pattern>& patterns);

std::vector<Pattern> searchMaxPatternsSlack(std::vector<std::string> const &sequences, size_t slack);



template<class Iterator>
//!
//! \brief searchMaxPatterns at least two sequences
//! \param begin
//! \param end
//! \return
//!
std::vector<Pattern> searchMaxPatternsSlack(Iterator begin, Iterator end, size_t slack) {
    Iterator it1 = begin++;
    Iterator it2 = begin++;
    auto td = getTraceInstance();
    size_t mergedSeq = 2;
    auto startChrono = std::chrono::high_resolution_clock::now();
    std::vector<Pattern> patterns = searchCommonPatterns(*it1, *it2, slack);
    patterns = removeIncluded(patterns);
    auto endChrono = std::chrono::high_resolution_clock::now();

    if(TraceDebug::patternRequired) {
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(endChrono - startChrono).count();
        td->writePatterns(mergedSeq, patterns.size(), time);
    }

    size_t num_threads = omp_get_max_threads();

    for( ; begin != end ; ++begin) {
        auto startChrono = std::chrono::high_resolution_clock::now();
        mergedSeq++;
        auto cAd = searchIncludedAndNotPatterns(*begin, patterns);
        std::vector<std::vector<Pattern>> patternsTmp{num_threads};
        size_t tid{};
        #pragma omp parallel for schedule(dynamic,1) private(tid)
        for(size_t i = 0; i < cAd.second.size(); ++i) {
            tid = omp_get_thread_num();
            std::vector<Pattern> v = searchCommonPatterns(*begin, cAd.second[i], slack);
            v = removeIncluded(v);
            // it seems to be faster to sort after all insertions...
            patternsTmp[tid].insert
                ( patternsTmp[tid].end()
                , std::make_move_iterator(v.begin()), std::make_move_iterator(v.end())
                );
        }
        for(auto i = 0; i < patternsTmp.size(); ++i) {
            cAd.first.insert
                ( cAd.first.end()
                , make_move_iterator(patternsTmp[i].begin())
                , make_move_iterator(patternsTmp[i].end())
                );
        }
        sort(cAd.first.begin(), cAd.first.end(), Pattern::grSizeTotal());
        removeDuplicates(cAd.first);
        patterns = removeIncluded(cAd.first);
        auto endChrono = std::chrono::high_resolution_clock::now();

        if(TraceDebug::patternRequired) {
            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(endChrono - startChrono).count();
            td->writePatterns(mergedSeq, patterns.size(), time);
        }
    }

    return patterns;
}

//!
//! \brief check2Blocks checks if there exist blocks of slack slack.
//! \param s A sequence of v, idealy the smallest one.
//! \param v A set of sequences.
//! \param slack The slack
//! \return
//!
bool check2Blocks(std::string const &s, std::vector<std::string> const &v, size_t slack = 0);

#if 0
//!
//! \brief decomposePatternsSlow
//! \param patterns
//! \param slack
//! \return
//!
std::vector<Pattern> decomposePatterns(std::vector<Pattern>& patterns, int slack);



#endif
}
#endif // ALIGNEMENT_H
