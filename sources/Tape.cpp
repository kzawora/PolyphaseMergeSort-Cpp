#include "Tape.hpp"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#pragma once

Tape::Tape(std::string _filePath, int mode)
    : filePath(_filePath), diskOpCounter(0), lastTapePos(0), seriesCount(0) {
    if (this->file.is_open() == false)
        this->OpenStream(mode);
}
Tape::~Tape() { file.close(); }

std::vector<double> Tape::GetNextBlock() {
    const size_t arraySize = BLOCK_SIZE / sizeof(double);
    int position = static_cast<int>(file.tellg());
    diskOpCounter++;          // DISKOP: file.tellg()
    double x[arraySize] = {}; // TODO: zera binarne?
    if (file && (position + BLOCK_SIZE <= fileSize))
        file.read(reinterpret_cast<char *>(&x), BLOCK_SIZE);
    else if (file)
        file.read(reinterpret_cast<char *>(&x), fileSize - position);
    diskOpCounter++; // DISKOP: file.read(...)
    return std::vector<double>(x, x + sizeof x / sizeof x[0]);
}
bool Tape::HasNextBlock() {
    int pos = readBlock.endInTape;
    if (pos < fileSize)
        return true;
    return false;
};

void Tape::OpenStream(int mode) {
    std::ifstream file_temp(filePath, std::ios::binary | std::ios::ate);
    this->fileSize = file_temp.tellg();
    diskOpCounter++; // DISKOP: file.tellg()
    file_temp.close();
    file.open(filePath, mode);
    diskOpCounter++; // DISKOP: file.open()
}
void Tape::CloseStream() { file.close(); }

Block Tape::BlockRead() {
    auto block = GetNextBlock();
    std::vector<Record> records;
    Record record;
    diskOpCounter++; // DISKOP: file.tellg()
    for (auto val : block) {
        if (!std::isnan(val))
            record.Push(val);
        else if (record.Size() != 0) {
            records.push_back(record);
            Record tmp;
            record = tmp;
        }
    }
    lastTapePos =
        static_cast<int>(file.tellg()) - record.Size() * sizeof(double);
    Block res(records);
    res.endInTape = (file.tellg());
    file.seekg(lastTapePos, std::ios_base::beg);
    //   diskOpCounter++;       // DISKOP: file.seekg()
    return res; // co zrobic z ostatnim rekordem?
}

long long Tape::GetDiskOpCount() { return this->diskOpCounter; }

void Tape::BlockWrite() {
    std::vector<double> result;
    for (auto val : writeBlock.GetValues()) {
        std::vector<double> toInsert = val.GetValues();
        result.insert(result.end(), toInsert.begin(), toInsert.end());
        result.push_back(SEPARATOR_VALUE);
    }
    double *toWrite = &result[0];
    file.write(reinterpret_cast<char *>(toWrite),
               result.size() * sizeof(double));
    diskOpCounter++; // DISKOP: file.write(...)
}
bool Tape::HasNext() {
    if (readBlock.HasNextRecord())
        return true;
    return this->HasNextBlock();
}
Record Tape::GetNext() {
    if (readBlock.GetSize() == 0 || readBlock.HasNextRecord() == false)
        this->readBlock = this->BlockRead();
    auto res = this->readBlock.GetNextRecord();
    this->inSeries =
        lastRecord.IsEmpty() || lastRecord <= res; // TODO: nierownosci
    this->lastRecord = res;
    return res;
}
Record Tape::GetCurrent() { return this->readBlock.GetCurrentRecord(); }
Record Tape::PeekNext() {
    auto block =
        (readBlock.GetSize() == 0 || readBlock.HasNextRecord() == false)
            ? this->BlockRead()
            : this->readBlock;

    auto res = block.PeekNextRecord();
    return res;
}

void Tape::ChangeMode(int mode) {
    this->CloseStream();
    this->OpenStream(mode);
    this->inSeries = true;
    this->lastTapePos = (0);
    this->seriesCount = (0);
    this->readBlock = Block();
    this->writeBlock = Block();
}

void Tape::Push(Record rec) {
    if (this->lastRecord.IsEmpty() ||
        this->lastRecord > rec) // TODO: nierownosci
        this->seriesCount++;

    this->lastRecord = rec;
    writeBlock.Push(rec);
    if (this->writeBlock.GetSizeInBytes() >= BLOCK_SIZE) {
        BlockWrite();
        writeBlock.Clear();
    }
}

std::string Tape::GetFilePath() { return this->filePath; };
