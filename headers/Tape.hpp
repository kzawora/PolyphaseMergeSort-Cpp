#include "Block.hpp"
#include "Record.hpp"
#include <fstream>
#include <vector>
#pragma once

#define BLOCK_SIZE 4096
#define DIST_UPPER_LIMIT 100.
#define DIST_LOWER_LIMIT -100.
#define SEPARATOR_VALUE std::nan("")
#define _DEBUG_ 0

class Tape {
    long long diskOpCounter;
    long long lastTapePos;
    size_t fileSize;
    std::fstream file;
    std::string filePath;
    std::vector<double> GetNextBlock();
    Block readBlock;
    Block writeBlock;
    bool HasNextBlock();
    Block BlockRead();
    bool isOpen;

  public:
    Tape(std::string _filePath, int);
    ~Tape();

    void OpenStream(int);
    void CloseStream();
    void GenerateTape(int);
    long long GetDiskOpCount();
    bool HasNext();
    Record GetNext();
    void Push(Record);
    void BlockWrite(Block);
    void WriteAll();
};
