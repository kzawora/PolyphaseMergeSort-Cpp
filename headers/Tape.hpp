#include "Block.hpp"
#include "Record.hpp"
#include <fstream>
#include <vector>
#pragma once

class Tape {
    // internal tape management variables
    long long diskOpCounter;
    long long lastTapePos;
    size_t fileSize;
    std::fstream file;
    std::string filePath;
    Record lastRecord;
    bool restore;
    int popCnt;

    // stream management
    void OpenStream(std::ios_base::openmode);
    void CloseStream();

    // block management
    Block readBlock;
    Block writeBlock;
    Block BlockRead();
    bool HasNextBlock();
    std::vector<double> GetNextBlock();

  public:
    // algorithm specific variables useful for programmer
    bool inSeries;
    size_t seriesCount;
    int dummies;

    // constructors/destructors
    Tape(std::string _filePath, std::ios_base::openmode);
    ~Tape();

    // stream management API
    void Clear();
    void ChangeMode(std::ios_base::openmode);
    long long GetDiskOpCount();
    int GetFileSize();
    
    // tape managment API
    bool HasNext();
    Record GetNext();
    void Push(Record);
    void BlockWrite(); // TODO: rename to flush()
    void Restore();
    std::string GetFilePath() const;

    friend std::ostream &operator<<(std::ostream &, const Tape &);
};
