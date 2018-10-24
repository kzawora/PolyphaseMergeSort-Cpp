#include "Block.hpp"
#include "Record.hpp"
#include <fstream>
#include <vector>
#pragma once

#define BLOCK_SIZE 4096
#define DIST_UPPER_LIMIT 100.
#define DIST_LOWER_LIMIT -100.
#define SEPARATOR_VALUE 0.
#define _DEBUG_ 0

class Tape {
    long long diskOpCounter;
    long long lastTapePos;
    size_t fileSize;
    std::fstream file;
    std::string filePath;

    std::vector<double> GetNextBlock();

  public:
    Tape(std::string _filePath);
    ~Tape();

    void OpenStream(int);
    void CloseStream();
    void GenerateTape(int);
    long long GetDiskOpCount();
    Block RecordBlockRead();
    void SaveData(std::vector<Record>);
};
