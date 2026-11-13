import std;
import IOMatrix;
import Multiplyer;
import RMatrix;
#include <omp.h>

int main() {
    constexpr std::array sizes = {200, 400, 800, 1200, 1600, 2000};
    constexpr std::array thread_counts = {1, 2, 4, 8};

    std::println("Running OpenMP Matrix Multiplication Experiments...\n");
    std::println("{:<10} | {:<10} | {:<15}", "Size", "Threads", "Time (ms)");
    std::println("{:-<10}-+-{:-<10}-+-{:-<15}", "", "", "");

    auto dataPath = [](int n, int idx) {
        return std::format("../../lab1/data/m{}_{}.txt", n, idx);
    };

    for (int size : sizes) {
        std::vector<int> a, b;
        std::vector<long long> c;

        bool loaded = VectorLoading(a, dataPath(size, 1))
                   && VectorLoading(b, dataPath(size, 2));

        if (!loaded) {
            a = generateRandomMatrix(size * size);
            b = generateRandomMatrix(size * size);
            VectorSaving(a, dataPath(size, 1));
            VectorSaving(b, dataPath(size, 2));
        }

        for (int t : thread_counts) {
            omp_set_num_threads(t);
            
            auto start = std::chrono::steady_clock::now();
            MultiplyerMatrix(a, b, c, size);
            auto end = std::chrono::steady_clock::now();
            
            double elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();
            std::println("{:<10} | {:<10} | {:<15.2f}", size, t, elapsed_ms);
        }
        std::println();
    }

    return 0;
}
