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

#define PRINT_TAPES 1

// for TapeGenerator
#define DIST_LOWER_LIMIT 0
#define DIST_UPPER_LIMIT 100
#include <memory>
#include <queue>
#pragma once
enum io {
    READONLY = std::ios::in | std::ios::binary,
    WRITEONLY = std::ios::out | std::ios::binary | std::ios::app,
};

class PMSTest2 {
    std::vector<std::shared_ptr<Tape>> Tapes;
    std::string filename;

  public:
    PMSTest2::PMSTest2(std::shared_ptr<Tape> inputTape) {}
    PMSTest2::PMSTest2(std::string _filename) : filename(_filename) {
        Tapes.push_back(std::make_shared<Tape>(_filename, READONLY));
        std::remove("t2.bin");
        Tapes.push_back(std::make_shared<Tape>("t2.bin", WRITEONLY));
        std::remove("t3.bin");
        Tapes.push_back(std::make_shared<Tape>("t3.bin", WRITEONLY));
    }
    int PMSTest2::Distribute() {
        int phases = 0;

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
        return phases;
    }
    void PMSTest2::Print() {
        std::cout << "PRINTING TAPES:" << std::endl;
        for (auto tape : Tapes) {
            std::cout << "==================== TAPE - CURRENT "
                      << tape->GetCurrent();
            std::cout << *tape << std::endl;
        }
    }

    void PMSTest2::Merge() {
        Tapes[0]->Clear();
        //       std::cout << std::endl << "NEW PHASE" << std::endl;
        //       Print();

        std::shared_ptr<Tape> Tape1 = Tapes[1], Tape2 = Tapes[2];
        Record record1, record2;
        // TODO: tu sie psuje
        // Tape1->ChangeMode(READONLY);
        // Tape2->ChangeMode(READONLY);

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
        /*
        std::cout << "RECORD1: " << record1 << std::endl;
        std::cout << "RECORD2: " << record2 << std::endl;
        */
        Tapes[0]->ChangeMode(READONLY);

        if (Tapes[1]->HasNext())
            std::swap(Tapes[0], Tapes[2]);
        else if (Tapes[2]->HasNext())
            std::swap(Tapes[0], Tapes[1]);
    }
    std::string PMSTest2::Sort() {
        //        Print();
        int phases = Distribute();
        Tapes[1]->ChangeMode(READONLY);
        Tapes[2]->ChangeMode(READONLY);
        //        Print();
        for (int i = 0; i < phases; i++)
            Merge();
        return Tapes[0]->GetFilePath();
    }
    PMSTest2::~PMSTest2() {}
};