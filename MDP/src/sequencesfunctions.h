#ifndef SEQUENCESFUNCTIONS_H
#define SEQUENCESFUNCTIONS_H


#include "matrix.h"
#include "pattern.h"
#include "utility.h"

#include <vector>
#include <string>
#include <memory>
#include "omp.h"

namespace mdp {
//!
//! \brief covered Return the sequences covered by at least one pattern of patterns
//! \param sequences
//! \param patterns
//! \return
//!
std::vector<std::string> support(std::vector<std::string> const &sequences, std::vector<Pattern> const &patterns);

//!
//! \brief exclusionMatrix compute the matrix of exclusion beteween sequences and patterns
//! \return m(s,p)
//!
matrix<bool> computeSupportMatrix(std::vector<std::string> const& sequences, std::vector<Pattern> const &patterns);

//!
//! \brief computeEmbeddings compute all the embeddings of each patterns in each sequences
//! \param sequences
//! \param patterns
//! \return
//!
matrix<std::vector<size_t>> computeEmbeddingsMatrix(std::vector<std::string> const& sequences, std::vector<Pattern> const &patterns);

//!
//! \brief computeFirstEmbeddingsMatrix
//! \param sequences
//! \param patterns
//! \return
//!
matrix<size_t> computeFirstEmbeddingsMatrix(std::vector<std::string> const& sequences, std::vector<Pattern> const &patterns);


//!
//! \brief beginSequences
//! \param sequences
//! \param patterns
//! \return if the pattern starts all the sequences
//!
std::vector<bool> startSequences(std::vector<std::string> const &sequences, std::vector<Pattern> const &patterns);

//!
//! \brief beginSequences
//! \param sequences
//! \param patterns
//! \return if the pattern starts all the sequences
//!
std::vector<bool> endSequences(std::vector<std::string> const &sequences, std::vector<Pattern> const &patterns);


std::vector<Pattern> decomposePatternsSlow(std::vector<Pattern>& patterns, const size_t slack);

}

#endif // SEQUENCESFUNCTIONS_H
