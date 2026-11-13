export module IOMatrix;
import std;

export bool VectorLoading(std::vector<int>& vec, const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    int number;
    while (file >> number) vec.push_back(number);
    return true;
}

export void VectorSaving(const std::vector<int>& vec, const std::string& path) {
    std::ofstream file(path);
    if (!file) throw std::logic_error("Error open file");

    for (int x : vec) std::println(file, "{}", x);
}

export void WriteResult(const std::vector<long long>& result, std::string_view filename,
                        std::size_t size, const auto& time) {
    std::ofstream file(std::format("../../../lab1/data/result/{}", filename));
    if (!file) throw std::logic_error("Error open file");

    std::println(file, "#Execution time: {}", time);
    std::println(file, "#Number of matrix elements: {}\n", size * size);

    for (std::size_t i = 0; i < size; ++i) {
        for (std::size_t j = 0; j < size; ++j)
            std::print(file, "{:12}", result[i * size + j]);
        std::println(file, "");
    }
    std::println("Matrix successfully written to file {}", filename);
}
