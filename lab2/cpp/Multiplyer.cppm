export module Multiplyer;

import std;
#include <omp.h>

export void MultiplyerMatrix(const std::vector<int>& a, const std::vector<int>& b,
                          std::vector<long long>& result, std::size_t size) {
    result.assign(size * size, 0);
    #pragma omp parallel for schedule(static)
    for (std::size_t i = 0; i < size; ++i)
        for (std::size_t k = 0; k < size; ++k) {
            long long a_ik = a[i * size + k];
            for (std::size_t j = 0; j < size; ++j)
                result[i * size + j] += a_ik * b[k * size + j];
        }
}
