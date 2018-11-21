#include "Record.hpp"
#include <vector>
#pragma once

class Block {
    std::vector<Record> values;
    int currentPos;
    size_t sizeInBytes;

  public:
    size_t endInTape;
    Block();
    explicit Block(std::vector<Record>);
    ~Block();
    std::vector<Record> GetValues();
    Record GetNextRecord();
    Record GetCurrentRecord();
    bool HasNextRecord();
    void Push(Record);
    size_t GetSize();
    size_t GetSizeInBytes();
    void Clear();
};