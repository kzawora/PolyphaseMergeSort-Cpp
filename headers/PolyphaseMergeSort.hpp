#include "Tape.hpp"
#include <memory>
#include <vector>
#pragma once

class PolyphaseMergeSort {
    std::vector<std::shared_ptr<Tape>> tapes;
    std::string filename;
    int phases{0};
    double elapsedTime{0.};
    int series{0};
    int elements;
    int inputSize;
    
  public:
    PolyphaseMergeSort(std::string _filename, int elems);
    int Distribute();
    void Print();
    void GenerateReport();
    void Merge();
    std::string Sort();
    ~PolyphaseMergeSort();
};