#include "Block.hpp"
#pragma once
Block::Block() : currentPos(-1){};
Block::Block(std::vector<Record> recs) : currentPos(-1) { this->values = recs; };
Block::~Block(){};
std::vector<Record> Block::GetValues() { return this->values; };
void Block::Push(Record val) { this->values.push_back(val); };
Record Block::GetNextRecord() {
    this->currentPos++;
    return this->GetCurrentRecord();
};
Record Block::GetCurrentRecord() {
    if (currentPos < 0)
        throw "No current record!";
    if (this->currentPos < this->values.size())
        return this->values[this->currentPos];
    throw "Block out of bounds!";
};
void Block::SetPos(size_t pos) { this->currentPos = pos; }
size_t Block::GetPos() { return this->currentPos; };
size_t Block::GetSize() { return this->values.size(); };