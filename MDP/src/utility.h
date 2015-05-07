#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>
#include <unordered_set>


//!
//! \brief removeDuplicates removes the duplicates between the pattern of patterns.
//! removeDuplicates sort the pattern accordin to the predicate pred.
//! \param patterns the list of patterns
//! \param pred sorting criteria
//!
template<class T, class Pred>
void removeDuplicates(std::vector<T>& datas, Pred pred) {
    auto it = unique(datas.begin(), datas.end(), pred);
    datas.resize(distance(datas.begin(), it));
}

template<class T>
//!
//! \brief removeDuplicates removes the duplicates between the pattern of patterns.
//! removeDuplicates assumes the list of patterns is already sorted.
//! \param patterns the list of patterns
//!
void removeDuplicates(std::vector<T>& datas) {
    removeDuplicates(datas, std::equal_to<T>());
}

template<class T, class Pred>
//!
//! \brief sortAndMegreVector merge all the vector of datas in the first vector of data
//! \param datas
//! \param pred
//!
void sortAndMegreVector(std::vector<std::vector<T>>& datas, Pred pred) {
    // sort all the vectors
    #pragma omp parallel for schedule(static)
    for(size_t i = 0; i < datas.size(); ++i) {
        std::sort(datas[i].begin(), datas[i].end(), pred);
    }

    // compute the number of patterns
    auto size = std::accumulate
        ( datas.begin(), datas.end(), 0,
          [] (size_t a,  std::vector<T> const &rhs) {
                return a + rhs.size();
            }
        );
    // move all the patterns in the first vector and saves the start and end of each vectors.
    datas.front().reserve(size); // if the size isn't reserved then the iterators might be
                                    // invalidate during the insertion of new elements
    std::vector<typename std::vector<T>::iterator> bounds ;
    bounds.push_back(datas.front().begin())      ;
    bounds.push_back(datas.front().end())        ;
    for(size_t i = 1; i < datas.size(); ++i) {
        copy
            ( std::make_move_iterator(datas[i].begin())
            , std::make_move_iterator(datas[i].end())
            , std::back_inserter(datas.front())
            );
        bounds.push_back(datas.front().end());
    }
    // merge all the parts of the vecotr in order to sort the whole vector
    // !! inplace_merge doesn't remove duplicates
    for(size_t i = 1; i < bounds.size()-1; ++i) {
        std::inplace_merge(bounds.front(), bounds[i], bounds[i+1], pred);
    }
    datas.resize(1);
}



template<class T>
//!
//! \brief sortAndMegreVector
//! \param datas
//!
void sortAndMegreVector(std::vector<std::vector<T>>& datas) {
    sortAndMegreVector(datas, std::less<T>());
}

std::unordered_set<std::string> split(const std::string &s, char delim);

#endif // UTILITY_H
