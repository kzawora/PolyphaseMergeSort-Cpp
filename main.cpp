#include "Record.hpp"
#include "Tape.hpp"
#include <iostream>
#pragma once

int main() {
    const size_t size = 2 * (1 << 10); // 1Mi
    Tape tape1("test.bin");
    Tape tape2("test2.bin");

    tape1.GenerateTape(size);

    tape1.OpenStream(std::ios::in | std::ios::binary);
    std::vector<std::vector<Record>> recs;
    do
        recs.push_back(tape1.RecordBlockRead());
    while (recs.back().size() != 0);
    recs.pop_back();

    tape1.CloseStream();

    int counter = 0;
    for (auto record : recs) {
        counter++;
        long long size = 0;
        for (auto val : record)
            size += (val.Size() + 1) * sizeof(double);
        std::cout << "==================================== BLOCK: " << counter
                  << ", RECORDS: " << record.size() << " (" << size
                  << " bytes) "
                  << " ====================================" << std::endl;
        for (auto val : record)
            std::cout << val;
    }

    tape2.OpenStream(std::ios::out | std::ios::binary);
    std::vector<Record> data;
    for (auto record : recs)
        for (auto val : record)
            data.push_back(val);
    tape2.SaveData(data);
    tape2.CloseStream();

    Record r1({1., 2., 3., 999999.});
    Record r2({.1, 2., 4., 6., 1., 2., 6., 1060., 2., 11111111111111.});
    std::cout << (r2 < r1) << std::endl;
    std::cout << "Disk ops: " << tape1.GetDiskOpCount() << std::endl;

    return 0;
}
