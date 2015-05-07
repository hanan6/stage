#if defined(_OPENMP)
    #include <omp.h>
#else
    #ifndef OMP_H
    #define OMP_H

    #include <cstddef>
    constexpr std::size_t omp_get_max_threads(void);
    constexpr std::size_t omp_get_thread_num(void);
    constexpr void        omp_set_num_threads(std::size_t num_threads);

    #endif // OMP_H
#endif
