/*
✎ Exercise 16.
Write a function that returns the total size of all *.dat files below a directory, skipping unreadable subdirectories (`directory_options::skip_permission_denied`).
*/

#include <filesystem>
#include <fstream>
#include <iostream>


size_t total_dat_size(const std::filesystem::path& dir) {
    using std::filesystem::recursive_directory_iterator;
    using std::filesystem::directory_options;
    size_t total_size = 0;
    for (const auto& entry : recursive_directory_iterator(dir, directory_options::skip_permission_denied)) {
        if (entry.is_regular_file() && entry.path().extension() == ".dat") {
            total_size += entry.file_size();
        }
    }
    return total_size;
}

int main() {
    std::filesystem::path root = std::filesystem::temp_directory_path() / "cpp_demo";
    std::filesystem::create_directories(root / "a" / "b");
    std::ofstream(root / "a" / "x.log") << "hello";
    std::ofstream(root / "a" / "b" / "y.dat") << "world!";
    std::cout << "Total size of .dat files: " << total_dat_size(root) << " bytes\n";
    std::filesystem::remove_all(root);
}