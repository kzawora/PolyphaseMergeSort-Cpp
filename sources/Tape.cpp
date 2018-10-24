#include "Tape.hpp"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#pragma once

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
Tape::Tape(std::string _filePath)
    : filePath(_filePath), diskOpCounter(0), lastTapePos(0) {}
Tape::~Tape() {}

void Tape::OpenStream(int mode) {
    std::ifstream file_temp(filePath, std::ios::binary | std::ios::ate);
    this->fileSize = file_temp.tellg();
    diskOpCounter++; // DISKOP: file.tellg()
    file_temp.close();
    file.open(filePath, mode);
    diskOpCounter++; // DISKOP: file.open()
}
void Tape::CloseStream() { file.close(); }

Block Tape::RecordBlockRead() {
    auto block = GetNextBlock();
    std::vector<Record> records;
    Record record;
    long long recordPosition =
        (static_cast<long long>(file.tellg()) - BLOCK_SIZE) / sizeof(double);
    diskOpCounter++; // DISKOP: file.tellg()
    long long counter = recordPosition, lastCounter = 0;
    for (auto val : block) {
        if (val != SEPARATOR_VALUE)
            record.Add(val);
        else if (record.Size() != 0) {
            record.recordPosition = counter;
            records.push_back(record);
            Record tmp;
            record = tmp;
        }
        counter++;
    }
    lastTapePos =
        static_cast<int>(file.tellg()) - record.Size() * sizeof(double);
    file.seekg(lastTapePos, std::ios_base::beg);
    diskOpCounter++;       // DISKOP: file.seekg()
    return Block(records); // co zrobic z ostatnim rekordem?
}

void Tape::GenerateTape(int size) {
    file.open(filePath, std::ios::out | std::ios::binary);
    std::default_random_engine gen(clock());
    std::uniform_real_distribution<double> dist(DIST_LOWER_LIMIT,
                                                DIST_UPPER_LIMIT);
    std::uniform_int_distribution<int> distInt(1, 15);
    auto generator = [&]() { return dist(gen); };
    auto generatorInt = [&]() { return distInt(gen); };
    for (int i = 0; i < size; i++) {
        int setSize = generatorInt();
        while (setSize--) {
            double num = generator();
            file.write(reinterpret_cast<char *>(&num), sizeof(double));
        }

        double separator = SEPARATOR_VALUE;
        file.write(reinterpret_cast<char *>(&separator), sizeof(double));
    }
    file.close();
}

long long Tape::GetDiskOpCount() { return this->diskOpCounter; }

void Tape::SaveData(std::vector<Record> recs) {
    std::vector<double> result;
    for (auto val : recs) {
        std::vector<double> toInsert = val.GetValues();
        result.insert(result.end(), toInsert.begin(), toInsert.end());
        result.push_back(SEPARATOR_VALUE);
    }
    double *toWrite = &result[0];
    file.write(reinterpret_cast<char *>(toWrite),
               result.size() * sizeof(double));
    diskOpCounter++; // DISKOP: file.write(...)
}
