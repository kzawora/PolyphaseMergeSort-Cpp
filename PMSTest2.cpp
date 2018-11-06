#include "CommonHeader.hpp"
#include "Record.hpp"
#include "Tape.hpp"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#pragma once

#include <memory>
#include <queue>
#pragma once
enum io {
    READONLY = std::ios::in | std::ios::binary,
    WRITEONLY = std::ios::out | std::ios::binary | std::ios::app,
};
#if PRINT_TAPES == 1
static int _phasecnt = 0;
#endif
class PMSTest2 {
    std::vector<std::shared_ptr<Tape>> Tapes;
    std::string filename;
    int phases;
    double elapsedTime;
    int series;
    int elements;

  public:
    PMSTest2::PMSTest2(std::string _filename, int elems)
        : filename(_filename), elements(elems) {
        Tapes.push_back(std::make_shared<Tape>(_filename, READONLY));
        std::remove("t2.bin");
        Tapes.push_back(std::make_shared<Tape>("t2.bin", WRITEONLY));
        std::remove("t3.bin");
        Tapes.push_back(std::make_shared<Tape>("t3.bin", WRITEONLY));
    }
    int PMSTest2::Distribute() {
        phases = 0;

        Tapes[0]->ChangeMode(READONLY);
        Tapes[1]->Clear();
        Tapes[2]->Clear();

        std::shared_ptr<Tape> src = Tapes[0], dest = Tapes[1];
        long long fib = 1, fibOld = 0, fibCopy = fib;
        while (src->HasNext()) {
            Record rec = src->GetNext();
            if (!src->inSeries &&
                dest->seriesCount == fib) { // TODO: ogarnac to
                dest = dest == Tapes[1] ? Tapes[2] : Tapes[1];
                fibCopy = fib;
                fib = fib + fibOld;
                fibOld = fibCopy;
                phases++;
            }
            dest->Push(rec);
        }
        dest->dummies = fib - dest->seriesCount;
        Tapes[1]->BlockWrite();
        Tapes[2]->BlockWrite();
        series = Tapes[1]->seriesCount + Tapes[2]->seriesCount;
        return phases;
    }
    void PMSTest2::Print() {
        for (auto tape : Tapes) {
            std::cout << "==== TAPE - " << tape->GetFilePath();
            if (tape->dummies > 0)
                std::cout << " (" << tape->dummies << " dummy series)";
            std::cout << " ====" << std::endl;
            if (tape->HasNext())
                std::cout << *tape << std::endl;
        }
    }
    void PMSTest2::GenerateReport() {
        //  2
        double diskopCalc = 2 * elements * (1 + 1) * sizeof(double) *
                              (1.04 * std::log2(series) + 1) / BLOCK_SIZE;
        int diskops = 0;
        for (auto tape : Tapes) {
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
                  << "THEORETICAL PHASES: "
                  << (1.45 * std::log2(series)) << std::endl;
        std::cout << "==== "
                  << "DISK OPERATIONS: " << diskops << std::endl;
        std::cout << "==== "
                  << "THEORETICAL DISKOPS: " << diskopCalc << std::endl;
        std::cout << "==== "
                  << "TIME: " << elapsedTime << " seconds" << std::endl;
    }
    void PMSTest2::Merge() {
        Tapes[0]->Clear();

        std::shared_ptr<Tape> Tape1 = Tapes[1], Tape2 = Tapes[2];
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
                if (record1.IsEmpty() ||
                    !record2.IsEmpty() && record1 > record2) {
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
                Tapes[0]->Push(min);
            }
        }
        Tapes[0]->BlockWrite();

#if PRINT_TAPES == 1
        std::cout << std::endl
                  << "======== MERGE AT PHASE " << ++_phasecnt
                  << " ========" << std::endl;
        Print();
#endif
        Tapes[0]->ChangeMode(READONLY);

        if (Tapes[1]->HasNext())
            std::swap(Tapes[0], Tapes[2]);
        else if (Tapes[2]->HasNext())
            std::swap(Tapes[0], Tapes[1]);
    }
    std::string PMSTest2::Sort() {
        clock_t begin = std::clock();

#if PRINT_TAPES == 1
        std::cout << "======== TAPES BEFORE DISTRIBUTION ========" << std::endl;
        Print();
#endif
        int phases = Distribute();
        Tapes[0]->Clear(); // to moze zepsuc cos
        Tapes[1]->ChangeMode(READONLY);
        Tapes[2]->ChangeMode(READONLY);

#if PRINT_TAPES == 1
        std::cout << "======== TAPES AFTER DISTRIBUTION ========" << std::endl;
        Print();
#endif
        for (int i = 0; i < phases; i++)
            Merge();
        clock_t end = std::clock();
        elapsedTime = double(end - begin) / CLOCKS_PER_SEC;
        GenerateReport();
        return Tapes[0]->GetFilePath();
    }
    PMSTest2::~PMSTest2() {}
};