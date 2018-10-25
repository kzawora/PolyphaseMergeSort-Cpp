#include "Record.hpp"
#include <vector>
#pragma once

class Block {
    std::vector<Record> values;
    size_t currentPos;

  public:
    size_t endInTape;
    Block();
    Block(std::vector<Record>);
    ~Block();
    std::vector<Record> GetValues();
    Record GetNextRecord();
    Record GetCurrentRecord();
    void SetPos(size_t);
    bool HasNextRecord();
    size_t GetPos();
    void Push(Record);
    size_t GetSize();
    size_t Block::GetSizeInBytes();
    void Clear();
};