export module RMatrix;

import std;

export std::vector<int> generateRandomMatrix(std::size_t count, int min = 0, int max = 100) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);

    std::vector<int> matrix(count);
    for (auto& x : matrix) x = dist(gen);
    return matrix;
}
