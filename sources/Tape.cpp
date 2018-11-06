#include "CommonHeader.hpp"
#pragma once

Tape::Tape(std::string _filePath, int mode)
    : filePath(_filePath), diskOpCounter(0), lastTapePos(0), seriesCount(0),
      restore(false), popCnt(0), dummies(0) {
    if (this->file.is_open() == false)
        this->OpenStream(mode);
}
Tape::~Tape() { file.close(); }

void Tape::Restore() { this->restore = true; }

std::vector<double> Tape::GetNextBlock() {
    const size_t arraySize = BLOCK_SIZE / sizeof(double);
    int position = static_cast<int>(file.tellg());
    double x[arraySize] = {}; // TODO: zera binarne?
    if (file && (position + BLOCK_SIZE <= fileSize)) {
        file.read(reinterpret_cast<char *>(&x), BLOCK_SIZE);
        diskOpCounter++; // DISKOP: file.read(...)
        reads++;    // TODO: remove this

    } else if (file) {
        file.read(reinterpret_cast<char *>(&x), fileSize - position);
        diskOpCounter++; // DISKOP: file.read(...)
        reads++;    // TODO: remove this
    }
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
    file_temp.close();
    file.open(filePath, mode);
}
void Tape::CloseStream() { file.close(); }

Block Tape::BlockRead() {
    auto block = GetNextBlock();
    std::vector<Record> records;
    Record record;
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
    if (result.size() > 0) {
        double *toWrite = &result[0];
        file.write(reinterpret_cast<char *>(toWrite),
                   result.size() * sizeof(double));
        file.flush();
        diskOpCounter++; // DISKOP: file.write(...)
        writes++;    // TODO: remove this
    }
}
bool Tape::HasNext() {
    if (restore)
        return true;
    if (readBlock.HasNextRecord())
        return true;
    return this->HasNextBlock();
}
Record Tape::GetNext() {
    if (restore) {
        restore = false;
        return this->lastRecord;
    }
    if (readBlock.GetSize() == 0 || readBlock.HasNextRecord() == false)
        this->readBlock = this->BlockRead();
    auto res = this->readBlock.GetNextRecord();
    this->inSeries =
        lastRecord.IsEmpty() || lastRecord <= res; // TODO: nierownosci
    this->lastRecord = res;
    popCnt++;
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
    this->restore = false;
    this->popCnt = 0;
}

void Tape::Push(Record rec) {
    if (this->lastRecord.IsEmpty() ||
        this->lastRecord > rec) // TODO: nierownosci
        this->seriesCount++;

    this->lastRecord = rec;
    writeBlock.Push(rec);
    auto size = this->writeBlock.GetSizeInBytes();
    if (size >= BLOCK_SIZE) {
        BlockWrite();
        writeBlock.Clear();
    }
}

void Tape::Clear() {
    this->CloseStream();
    std::remove(filePath.c_str());
    this->OpenStream(std::ios::out | std::ios::binary | std::ios::app);
    this->inSeries = true;
    this->lastTapePos = (0);
    this->seriesCount = (0);
    this->readBlock = Block();
    this->writeBlock = Block();
    this->restore = false;
    this->popCnt = 0;
}

std::string Tape::GetFilePath() const { return this->filePath; }

std::ostream &operator<<(std::ostream &os, const Tape &tp) {
    std::string name = tp.GetFilePath();
    auto tape = std::make_shared<Tape>(name, std::ios::in | std::ios::binary);

    long long counter = 1;
    long long counter2 = 1;
    while (tape->HasNext()) {
        auto x = tape->GetNext();
        // if (++counter + (int)tp.restore > tp.popCnt) {
        if (++counter > tp.popCnt) {
#if PRINT_SERIES == 1
            if (tape->inSeries == false && counter2 != 1)
                os << "SERIES BREAK" << std::endl;
#endif
            os << counter2++ << '\t' << x;
        }
    }
    return os;
}
