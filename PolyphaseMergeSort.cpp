#include "Tape.hpp"
#include <iostream>
#include <vector>
#pragma once
enum io {
    READONLY = std::ios::in | std::ios::binary,
    WRITEONLY = std::ios::out | std::ios::binary | std::ios::app,
    READWRITE = std::ios::out | std::ios::binary | std::ios::app
};

class PolyphaseMergeSort {
    std::vector<std::shared_ptr<Tape>> tapes;

  public:
    PolyphaseMergeSort(std::shared_ptr<Tape> inputTape) {}
    PolyphaseMergeSort(std::string filename) {

        tapes.push_back(std::make_shared<Tape>(filename, READONLY));
        std::remove("t2.bin");
        tapes.push_back(std::make_shared<Tape>("t2.bin", READWRITE));
        std::remove("t3.bin");
        tapes.push_back(std::make_shared<Tape>("t3.bin", READWRITE));
    }
    void Distribute() {
        tapes[0]->ChangeMode(READONLY);
        tapes[1]->ChangeMode(WRITEONLY);
        tapes[2]->ChangeMode(WRITEONLY);
        std::shared_ptr<Tape> src = tapes[0], dest = tapes[1];
        long long fib = 1, fibOld = 0, fibCopy = fib;
        while (src->HasNext()) {
            Record rec = src->GetNext();
            if (!src->inSeries &&
                dest->seriesCount == fib) { // TODO: ogarnac to
                dest = dest == tapes[1] ? tapes[2] : tapes[1];
                fibCopy = fib;
                fib = fib + fibOld;
                fibOld = fibCopy;
            }
            dest->Push(rec);
        }
        dest->dummies = fib - dest->seriesCount;
        tapes[1]->BlockWrite();
        tapes[2]->BlockWrite();
    }
    void Print() {
        tapes[0]->ChangeMode(READONLY);
        tapes[1]->ChangeMode(READONLY);
        tapes[2]->ChangeMode(READONLY);
        
        long long counter = 0;
        std::cout << "==================== TAPE 0 ===================="
                  << std::endl;
        while (tapes[0]->HasNext()) {
            auto x = tapes[0]->GetNext();
            if (tapes[0]->inSeries == false)
                std::cout << "SERIES BREAK" << std::endl;
            std::cout << ++counter << '\t' << x;
        }
        counter = 0;
        std::cout << "==================== TAPE 1 ===================="
                  << std::endl;
        while (tapes[1]->HasNext()) {
            auto x = tapes[1]->GetNext();
            if (tapes[1]->inSeries == false)
                std::cout << "SERIES BREAK" << std::endl;
            std::cout << ++counter << '\t' << x;
        }
        counter = 0;
        std::cout << "==================== TAPE 2 ===================="
                  << std::endl;
        while (tapes[2]->HasNext()) {
            auto x = tapes[2]->GetNext();
            if (tapes[2]->inSeries == false)
                std::cout << "SERIES BREAK" << std::endl;
            std::cout << ++counter << '\t' << x;
        }
    }
    void Merge() {
        tapes[0]->CloseStream();
        std::remove(tapes[0]->GetFilePath().c_str());
        tapes[0]->ChangeMode(WRITEONLY);

        std::shared_ptr<Tape> tape1 = tapes[1], tape2 = tapes[2];
        Record record1, record2;
        tape1->ChangeMode(READONLY);
        tape2->ChangeMode(READONLY);

        while (tape1->HasNext() && tape2->HasNext()) {
            // update()
            if (tape1->dummies > 0) {
                record1 = Record();
                tape1->dummies--;
            } else {
                record1 = tape1->GetNext();
            }
            if (tape2->dummies > 0) {
                record2 = Record();
                tape2->dummies--;
            } else {
                record2 = tape2->GetNext();
            }

            // TODO: to moze byc zle
            auto GetMinimum = [&]() {
                Record minimum;
                if (record1.IsEmpty() && record2.IsEmpty()) {
                    return Record();
                }
                if (record1.IsEmpty() ||
                    !record2.IsEmpty() && record1 > record2) {
                    minimum = record2;
                    record2 = (tape2->GetCurrent() > tape2->PeekNext())
                                  ? Record()
                                  : tape2->GetNext();
                } else {
                    minimum = record1;
                    record1 = (tape1->GetCurrent() > tape1->PeekNext())
                                  ? Record()
                                  : tape1->GetNext();
                }
                return minimum;
            };

            while (true) {
                Record current = GetMinimum();
                if (current.IsEmpty())
                    break;
                tapes[0]->Push(current);
            }
        }
        tapes[0]->BlockWrite();
        tape1->BlockWrite();
        tape2->BlockWrite();
        auto temp = tapes[0];
        // flush temp
        temp->ChangeMode(READWRITE);

        if (tapes[1]->HasNext()) {
            tapes[0] = tapes[2];
            tapes[2] = temp;
        } else {
            tapes[0] = tapes[1];
            tapes[1] = temp;
        }
    }
    ~PolyphaseMergeSort() {}
};