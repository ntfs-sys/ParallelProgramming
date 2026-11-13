#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <random>
#include <string>
#include <format>
#include <array>
#include <cuda_runtime.h>

// CUDA ядро для умножения матриц
__global__ void matMulKernel(const int* A, const int* B, long long* C, int N) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < N && col < N) {
        long long sum = 0;
        for (int k = 0; k < N; ++k) {
            sum += (long long)A[row * N + k] * B[k * N + col];
        }
        C[row * N + col] = sum;
    }
}

// Вспомогательные функции для I/O и генерации данных (стиль lab1)
bool loadVector(std::vector<int>& vec, const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    int number;
    while (file >> number) vec.push_back(number);
    return true;
}

void saveVector(const std::vector<int>& vec, const std::string& path) {
    std::ofstream file(path);
    if (!file) throw std::logic_error("Error open file");

    for (int x : vec) std::println(file, "{}", x);
}

std::vector<int> generateRandomMatrix(std::size_t count, int min = 0, int max = 100) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);

    std::vector<int> matrix(count);
    for (auto& x : matrix) x = dist(gen);
    return matrix;
}

void saveResult(const std::vector<long long>& result, std::string_view filename,
                std::size_t size, double elapsed_ms) {
    std::ofstream file(std::format("../../../lab4/data/result/{}", filename));
    if (!file) throw std::logic_error("Error open file");

    std::println(file, "#Execution time: {}", elapsed_ms);
    std::println(file, "#Number of matrix elements: {}\n", size * size);

    for (std::size_t i = 0; i < size; ++i) {
        for (std::size_t j = 0; j < size; ++j)
            std::print(file, "{:12}", result[i * size + j]);
        std::println(file, "");
    }
    std::println("Matrix successfully written to file {}", filename);
}

int main() {
    constexpr std::array<int, 6> sizes = {200, 400, 800, 1200, 1600, 2000};
    constexpr std::array<int, 2> block_configs = {16, 32}; // Различные конфигурации блоков

    std::println("Running CUDA Matrix Multiplication Experiments...\n");
    std::println("{:<10} | {:<10} | {:<10} | {:<15}", "Size", "BlockX", "BlockY", "Time (ms)");
    std::println("{:-<10}-+-{:-<10}-+-{:-<10}-+-{:-<15}", "", "", "", "");

    auto dataPath = [](int n, int idx) {
        return std::format("../../../lab4/data/m{}_{}.txt", n, idx);
    };

    for (int size : sizes) {
        std::vector<int> h_A, h_B;

        bool loaded = loadVector(h_A, dataPath(size, 1))
                   && loadVector(h_B, dataPath(size, 2));

        if (!loaded) {
            h_A = generateRandomMatrix(size * size);
            h_B = generateRandomMatrix(size * size);
            saveVector(h_A, dataPath(size, 1));
            saveVector(h_B, dataPath(size, 2));
        }

        // Выделение памяти на устройстве
        int *d_A, *d_B;
        long long *d_C;
        cudaMalloc(&d_A, size * size * sizeof(int));
        cudaMalloc(&d_B, size * size * sizeof(int));
        cudaMalloc(&d_C, size * size * sizeof(long long));

        cudaMemcpy(d_A, h_A.data(), size * size * sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy(d_B, h_B.data(), size * size * sizeof(int), cudaMemcpyHostToDevice);

        for (int bs : block_configs) {
            dim3 block(bs, bs);
            dim3 grid((size + block.x - 1) / block.x, (size + block.y - 1) / block.y);

            cudaDeviceSynchronize(); // Синхронизация перед замером времени
            
            auto start = std::chrono::steady_clock::now();
            matMulKernel<<<grid, block>>>(d_A, d_B, d_C, size);
            cudaDeviceSynchronize();
            auto end = std::chrono::steady_clock::now();
            
            double elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();
            
            std::println("{:<10} | {:<10} | {:<10} | {:<15}", size, bs, bs, elapsed_ms);

            // Копируем результат обратно на хост только для первой конфигурации блока,
            // чтобы избежать лишнего трафика PCIe при экспериментах
            if (bs == block_configs[0]) {
                std::vector<long long> h_C(size * size);
                cudaMemcpy(h_C.data(), d_C, size * size * sizeof(long long), cudaMemcpyDeviceToHost);
                saveResult(h_C, std::format("result{}.txt", size), size, elapsed_ms);
            }
        }

        cudaFree(d_A);
        cudaFree(d_B);
        cudaFree(d_C);
    }

    return 0;
}
