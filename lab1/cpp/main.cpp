import std;
import IOMatrix;
import Multiplyer;
import RMatrix;

int main() {
    constexpr std::array sizes = {200, 400, 800, 1200, 1600, 2000};

    for (int size : sizes) {
        std::vector<int> a, b;
        std::vector<long long> c;

        auto dataPath = [](int n, int idx) {
            return std::format("../../../lab1/data/m{}_{}.txt", n, idx);
        };

        bool loaded = VectorLoading(a, dataPath(size, 1))
                   && VectorLoading(b, dataPath(size, 2));

        if (!loaded) {
            a = generateRandomMatrix(size * size);
            b = generateRandomMatrix(size * size);
            VectorSaving(a, dataPath(size, 1));
            VectorSaving(b, dataPath(size, 2));
        }

        auto start = std::chrono::steady_clock::now();
        MultiplyerMatrix(a, b, c, size);
        auto end = std::chrono::steady_clock::now();
        
        double elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();
        WriteResult(c, std::format("result{}.txt", size), size, elapsed_ms);
    }

    return 0;
}
