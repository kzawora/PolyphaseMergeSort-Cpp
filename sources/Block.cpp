#include "CommonHeader.hpp"
#pragma once
Block::Block() : currentPos(-1), endInTape(0), sizeInBytes(0){};
Block::Block(std::vector<Record> recs)
    : currentPos(-1), endInTape(0), sizeInBytes(0) {
    this->values = recs;
};
Block::~Block(){};
std::vector<Record> Block::GetValues() { return this->values; };
void Block::Push(Record val) {
    sizeInBytes += (val.GetValues().size() + 1) * sizeof(double);
    this->values.push_back(val);
};
Record Block::GetNextRecord() {
    this->currentPos++;
    return this->GetCurrentRecord();
};
Record Block::PeekNextRecord() {
    auto res = GetNextRecord();
    this->currentPos--;
    return res;
}
Record Block::GetCurrentRecord() {
    if (currentPos < 0)
        throw "No current record!";
    if (this->currentPos < this->values.size())
        return this->values[this->currentPos];
    //    throw "Block out of bounds!";
    return Record();
};
void Block::SetPos(size_t pos) { this->currentPos = pos; }
size_t Block::GetPos() { return this->currentPos; };
size_t Block::GetSize() { return this->values.size(); };
size_t Block::GetSizeInBytes() { return this->sizeInBytes; }

bool Block::HasNextRecord() { return currentPos + 1 < this->values.size(); }
void Block::Clear() {
    this->values.clear();
    this->sizeInBytes = 0;
}