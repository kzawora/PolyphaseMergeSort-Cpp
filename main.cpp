#include "PolyphaseMergeSort.hpp"
#include "helpers.cpp"
#include <iostream>

int main(int argc, char *argv[]) {
    std::string filename;
    int size;
    if (argc > 1) {
        if (std::string(argv[1]) == "-i") {
            if (argc > 2)
                filename = std::string(argv[2]);
            else {
                std::cout << "No filename provided!";
                return -1;
            }
            size = InputFromConsole(filename);
        } else if (std::string(argv[1]) == "-g")
            if (argc > 2) {
                filename = std::string(argv[2]);
                if (argc > 3)
                    size = std::stoi(std::string(argv[3]));
                else {
                    std::cout << "No size provided!";
                    return -1;
                }
                GenerateTape(filename, size);
            } else {
                std::cout << "No filename provided!";
                return -1;
            }
    } else { // temporary, for easy debugging
        //        std::cout << "No command line arguments provided!";
        //        return -1;
        filename = "test.bin";
        for (auto i = 1; i <= 10000000; i = i * 10) {
            GenerateTape(filename, i);
            PolyphaseMergeSort x(filename, i);
            std::string sortedPath = x.Sort();
            IsSorted(sortedPath);
        }
        return 0;
    }
    PolyphaseMergeSort x(filename, size);
    std::string sortedPath = x.Sort();
    IsSorted(sortedPath);
    return 0;
}
