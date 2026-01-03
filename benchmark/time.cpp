#include <iostream>
#include <string>
#include <chrono>
#include <cstdlib> 

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " \"<command>\"" << std::endl;
        std::cerr << "Example: " << argv[0] << " \"ls -lh\"" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    std::cout << "-> Executing command: " << command << std::endl << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    int result = std::system(command.c_str());

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    
    if (result != 0) {
        std::cout << "(Note: Command returned a non-zero exit code: " << result << ")" << std::endl;
    }

    std::cout << std::endl << "Elapsed time: " << elapsed.count() * 1000 << "ms" << std::endl;

    return 0;
}