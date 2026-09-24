import std;
import IOMatrix;
import Multiplyer;
import RMatrix;

int main() {
    constexpr std::array sizes = {200, 400, 800, 1200, 1600, 2000};

    std::println("Choose matrix size: 200, 400, 800, 1200, 1600 or 2000");

    int size = 0;
    while (!std::ranges::contains(sizes, size)) {
        std::cin >> size;
        if (!std::ranges::contains(sizes, size))
            std::println("Enter a valid matrix size from the list");
    }

    auto dataPath = [](int n, int idx) {
        return std::format("../../../lab1/data/m{}_{}.txt", n, idx);
    };

    std::vector<int> a, b;
    std::vector<long long> c;

    bool loaded = VectorLoading(a, dataPath(size, 1))
               && VectorLoading(b, dataPath(size, 2));

    if (!loaded) {
        std::println("Files not found - generating random matrices");
        a = generateRandomMatrix(size * size);
        b = generateRandomMatrix(size * size);
        VectorSaving(a, dataPath(size, 1));
        VectorSaving(b, dataPath(size, 2));
    } else {
        std::println("Matrices loaded from files");
    }

    auto start = std::chrono::steady_clock::now();
    MultiplyerMatrix(a, b, c, size);
    auto end = std::chrono::steady_clock::now();

    WriteResult(c, std::format("result{}.txt", size), size, end - start);

    return 0;
}