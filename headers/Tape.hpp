#include "Block.hpp"
#include "Record.hpp"
#include <fstream>
#include <vector>
#pragma once

#define BLOCK_SIZE 16 * (1 << 10)
#define WRITE_BLOCK_SIZE (1 << 20)

#define SEPARATOR_VALUE std::nan("")
#define _DEBUG_ 1

class Tape {
    long long diskOpCounter;
    long long lastTapePos;
    size_t fileSize;
    std::fstream file;
    std::string filePath;
    int mode;
    std::vector<double> GetNextBlock();
    Block readBlock;
    Block writeBlock;
    bool HasNextBlock();
    Block BlockRead();
    bool isOpen;
    void OpenStream(int);
    Record lastRecord;

  public:
    bool inSeries;
    size_t seriesCount;
    int dummies;
    void CloseStream();

    void WipeRead(int);
    void WipeWrite(int);
    void WipeAll(int);


    Tape(std::string _filePath, int);
    ~Tape();
    void ChangeMode(int);
    long long GetDiskOpCount();
    bool HasNext();
    std::string GetFilePath();
    Record GetNext();
    Record PeekNext();
    Record GetCurrent();
    void Push(Record);
    void BlockWrite();
};
