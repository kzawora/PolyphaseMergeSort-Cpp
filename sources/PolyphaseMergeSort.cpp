#include "PolyphaseMergeSort.hpp"
#include "CommonHeader.hpp"
#include "Record.hpp"
#include "Tape.hpp"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <random>
#include <string>
#include <vector>

#if PRINT_TAPES == 1
static int _phasecnt = 0;
#endif

PolyphaseMergeSort::PolyphaseMergeSort(std::string _filename, int elems)
    : filename(_filename), elements(elems) {
    tapes.push_back(std::make_shared<Tape>(_filename, READONLY));
    std::remove("t2.bin");
    tapes.push_back(std::make_shared<Tape>("t2.bin", WRITEONLY));
    std::remove("t3.bin");
    tapes.push_back(std::make_shared<Tape>("t3.bin", WRITEONLY));
}
int PolyphaseMergeSort::Distribute() {
    phases = 0;

    tapes[0]->ChangeMode(READONLY);
    inputSize = tapes[0]->GetFileSize();
    tapes[1]->Clear();
    tapes[2]->Clear();

    std::shared_ptr<Tape> src = tapes[0], dest = tapes[1];
    long long fib = 1, fibOld = 0, fibCopy;
    while (src->HasNext()) {
        Record rec = src->GetNext();
        if (!src->inSeries && dest->seriesCount == fib) {
            dest = dest == tapes[1] ? tapes[2] : tapes[1];
            fibCopy = fib;
            fib = fib + fibOld;
            fibOld = fibCopy;
            phases++;
        }
        dest->Push(rec);
    }
    dest->dummies = static_cast<int>(fib - dest->seriesCount);
    tapes[1]->BlockWrite();
    tapes[2]->BlockWrite();
    series = static_cast<int>(tapes[1]->seriesCount + tapes[2]->seriesCount);
    return phases;
}
void PolyphaseMergeSort::Print() {
    for (const auto &tape : tapes) {
        std::cout << "==== TAPE - " << tape->GetFilePath();
        if (tape->dummies > 0)
            std::cout << " (" << tape->dummies << " dummy series)";
        std::cout << " ====" << std::endl;
        if (tape->HasNext())
            std::cout << *tape << std::endl;
    }
}
void PolyphaseMergeSort::GenerateReport() {
    double diskopCalc =
 //       2 * elements * (((RECORD_MIN_SIZE + RECORD_MAX_SIZE) / 2.) + 1) * sizeof(double) *
        2 * inputSize *
        (1.04 * std::log2(series) + 1) / BLOCK_SIZE;
    int diskops = 0;
    for (const auto &tape : tapes) {
        diskops += tape->GetDiskOpCount();
    }
    std::cout << std::endl
              << "======== REPORT AFTER SORTING ========" << std::endl;
    std::cout << "==== "
              << "SIZE: " << elements << " records" << std::endl;
    std::cout << "==== "
              << "BLOCK SIZE: " << BLOCK_SIZE << " bytes" << std::endl;
    std::cout << "==== "
              << "SERIES COUNT: " << series << std::endl;
    std::cout << "==== "
              << "PHASES: " << phases << std::endl;
    std::cout << "==== "
              << "THEORETICAL PHASES: " << (1.45 * std::log2(series))
              << std::endl;
    std::cout << "==== "
              << "DISK OPERATIONS: " << diskops << std::endl;
    std::cout << "==== "
              << "THEORETICAL DISKOPS: " << diskopCalc << std::endl;
    std::cout << "==== "
              << "TIME: " << elapsedTime << " seconds" << std::endl;
}
void PolyphaseMergeSort::Merge() {
    tapes[0]->Clear();

    std::shared_ptr<Tape> Tape1 = tapes[1], Tape2 = tapes[2];
    Record record1, record2;

    while (Tape1->HasNext() && Tape2->HasNext()) {
        if (Tape1->dummies > 0) {
            record1 = Record();
            Tape1->dummies--;
        } else {
            record1 = Tape1->GetNext();
        }
        if (Tape2->dummies > 0) {
            record2 = Record();
            Tape2->dummies--;
        } else {
            record2 = Tape2->GetNext();
        }

        auto GetMinimum = [&]() {
            Record minimum;
            if (record1.IsEmpty() && record2.IsEmpty()) {
                return Record();
            }
            if (record1.IsEmpty() || !record2.IsEmpty() && record1 > record2) {
                minimum = record2;
                if (Tape2->HasNext()) {
                    record2 = Tape2->GetNext();
                    if (!Tape2->inSeries) {
                        record2 = Record();
                        Tape2->Restore();
                    }
                } else
                    record2 = Record();
            } else {
                minimum = record1;
                if (Tape1->HasNext()) {
                    record1 = Tape1->GetNext();
                    if (!Tape1->inSeries) {
                        record1 = Record();
                        Tape1->Restore();
                    }
                } else
                    record1 = Record();
            }
            return minimum;
        };
        while (true) {
            Record min = GetMinimum();
            if (min.IsEmpty())
                break;
            tapes[0]->Push(min);
        }
    }
    tapes[0]->BlockWrite();

#if PRINT_TAPES == 1
    std::cout << std::endl
              << "======== MERGE AT PHASE " << ++_phasecnt
              << " ========" << std::endl;
    Print();
#endif
    tapes[0]->ChangeMode(READONLY);

    if (tapes[1]->HasNext())
        std::swap(tapes[0], tapes[2]);
    else if (tapes[2]->HasNext())
        std::swap(tapes[0], tapes[1]);
}
std::string PolyphaseMergeSort::Sort() {
    clock_t begin = std::clock();

#if PRINT_TAPES == 1
    std::cout << "======== TAPES BEFORE DISTRIBUTION ========" << std::endl;
    Print();
#endif
    int phases = Distribute();
    tapes[0]->Clear(); // to moze zepsuc cos
    tapes[1]->ChangeMode(READONLY);
    tapes[2]->ChangeMode(READONLY);

#if PRINT_TAPES == 1
    std::cout << "======== TAPES AFTER DISTRIBUTION ========" << std::endl;
    Print();
#endif
    for (int i = 0; i < phases; i++)
        Merge();
    clock_t end = std::clock();
    elapsedTime = double(end - begin) / CLOCKS_PER_SEC;
    GenerateReport();
    return tapes[0]->GetFilePath();
}
PolyphaseMergeSort::~PolyphaseMergeSort() = default;
