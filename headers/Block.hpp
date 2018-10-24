#include "Record.hpp"
#include <vector>
#pragma once

class Block {
    std::vector<Record> values;
    size_t currentPos;

  public:
    Block();
    Block(std::vector<Record>);
    ~Block();
    std::vector<Record> GetValues();
    Record GetNextRecord();
    Record GetCurrentRecord();
    void SetPos(size_t);
    size_t GetPos();
    void Push(Record);
    size_t GetSize();
};