#include "Block.hpp"
#include "Record.hpp"
#include <fstream>
#include <vector>
#pragma once

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
    std::string GetFilePath() const;
    Record GetNext();
    Record PeekNext();
    Record GetCurrent();
    void Push(Record);
    void BlockWrite();

    friend std::ostream &operator<<(std::ostream &, const Tape &);
};
