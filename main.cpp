#include "Block.hpp"
#include "PolyphaseMergeSort.cpp"
#include "Record.hpp"
#include "Tape.hpp"
#include <iostream>
#pragma once

int main() {
    /*
    const size_t size = 1 * (1 << 10); // 1Mi
    Tape tape1("test.bin");
    Tape tape2("test1.bin");
    tape1.GenerateTape(size);

    PolyphaseMergeSort p("test.bin");
    tape1.OpenStream(std::ios::in | std::ios::binary);
    std::vector<Block> blocks;
    do
        blocks.push_back(tape1.RecordBlockRead());
    while (blocks.back().GetSize() != 0);
    blocks.pop_back();

    tape1.CloseStream();

    int counter = 0;
    for (auto record : blocks) {
        counter++;
        long long size = 0;
        for (auto val : record.GetValues())
            size += (val.Size() + 1) * sizeof(double);
        std::cout << " ==================================== BLOCK: " << counter
                  << ", RECORDS: " << record.GetSize() << " (" << size
                  << " bytes) "
                  << " ====================================" << std::endl;
        for (auto val : record.GetValues())
            std::cout << val;
    }

    tape2.OpenStream(std::ios::out | std::ios::binary);
    std::vector<Record> data;
    for (auto block : blocks)
        for (auto record : block.GetValues())
            data.push_back(record);
    tape2.SaveData(data);
    tape2.CloseStream();
    std::cout << "Disk ops: " << tape1.GetDiskOpCount() << std::endl;

*/
    Tape tape1("test.bin", std::ios::binary | std::ios::in);
    tape1.GenerateTape(1000);
    int counter = 0;
    while (tape1.HasNext()) {
        std::cout << ++counter << '\t' << tape1.GetNext();
    }

    /*
        Record r1({1., 2., 3., 999999.});
        Record r2({.1, 2., 4., 6., 1., 2., 6., 1060., 2., 11111111111111.});
        // std::cout << (r2 < r1) << std::endl;

        Block b({r1, r2, r2, r1});
        while (b.HasNext()) {
            std::cout << b.GetNextRecord();
            std::cout << "HasNext: " << b.HasNext() << std::endl;
        }
        



        b.Push(r1);
        b.Push(r2);
        while (b.HasNext()) {
            std::cout << b.GetNextRecord();
            std::cout << "HasNext: " << b.HasNext() << std::endl;
        }
    return 0;
    */
    return 0;
}
