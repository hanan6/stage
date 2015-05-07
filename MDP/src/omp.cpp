#include "omp.h"

#if !defined(_OPENMP)
constexpr std::size_t omp_get_max_threads(void) {
    return 1;
}

constexpr std::size_t omp_get_thread_num(void) {
    return 0;
}

constexpr void omp_set_num_threads(std::size_t num_threads) {
}
#endif
