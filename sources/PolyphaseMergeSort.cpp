#include "CommonHeader.hpp"
#pragma once
enum io {
    READONLY = std::ios::in | std::ios::binary,
    WRITEONLY = std::ios::out | std::ios::binary | std::ios::app,
};

PolyphaseMergeSort::PolyphaseMergeSort(std::shared_ptr<Tape> inputTape) {}
PolyphaseMergeSort::PolyphaseMergeSort(std::string _filename)
    : filename(_filename) {

    tapes.push_back(std::make_shared<Tape>(_filename, READONLY));
    std::remove("t2.bin");
    tapes.push_back(std::make_shared<Tape>("t2.bin", WRITEONLY));
    std::remove("t3.bin");
    tapes.push_back(std::make_shared<Tape>("t3.bin", WRITEONLY));
}
int PolyphaseMergeSort::Distribute() {
    int phases = 0;

    tapes[0]->ChangeMode(READONLY);
    tapes[1]->ChangeMode(WRITEONLY);
    tapes[2]->ChangeMode(WRITEONLY);
    std::shared_ptr<Tape> src = tapes[0], dest = tapes[1];
    long long fib = 1, fibOld = 0, fibCopy = fib;
    while (src->HasNext()) {
        Record rec = src->GetNext();
        if (!src->inSeries && dest->seriesCount == fib) { // TODO: ogarnac to
            dest = dest == tapes[1] ? tapes[2] : tapes[1];
            fibCopy = fib;
            fib = fib + fibOld;
            fibOld = fibCopy;
            phases++;
        }
        dest->Push(rec);
    }
    dest->dummies = fib - dest->seriesCount;
    tapes[1]->BlockWrite();
    tapes[2]->BlockWrite();
    return phases;
}
void PolyphaseMergeSort::Print() {
    std::cout << "PRINTING TAPES:" << std::endl;
//    for (auto tape : tapes) {
        std::cout << "==================== TAPE - CURRENT "
                  << tapes[1]->GetCurrent();
        std::cout << *tapes[1] << std::endl;
//    }
}
void PolyphaseMergeSort::Merge() {
    tapes[0]->CloseStream();
    std::remove(tapes[0]->GetFilePath().c_str());
    tapes[0]->ChangeMode(WRITEONLY);

    std::shared_ptr<Tape> tape1 = tapes[1], tape2 = tapes[2];
    Record record1, record2;
    // TODO: tu sie psuje
    // tape1->ChangeMode(READONLY);
    // tape2->ChangeMode(READONLY);

    while (tape1->HasNext() && tape2->HasNext()) {
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

        auto GetMinimum = [&]() {
            Record minimum;
            if (record1.IsEmpty() && record2.IsEmpty()) {
                return Record();
            }
            if (record1.IsEmpty() || !record2.IsEmpty() && record1 > record2) {
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
            Record min = GetMinimum();
            if (min.IsEmpty())
                break;
            tapes[0]->Push(min);
        }
    }
    tapes[0]->BlockWrite();
    std::cout << std::endl << "NEW PHASE" << std::endl;
//    Print();
    tapes[0]->ChangeMode(READONLY);

    if (tapes[1]->HasNext())
        std::swap(tapes[0], tapes[2]);
    else
        std::swap(tapes[0], tapes[1]);
}
void PolyphaseMergeSort::Sort() {
    int phases = Distribute();
    tapes[1]->ChangeMode(READONLY);
    tapes[2]->ChangeMode(READONLY);

    for (int i = 0; i < phases; i++)
        Merge();
}
PolyphaseMergeSort::~PolyphaseMergeSort() {}
