#include "Record.hpp"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#pragma once

// for TapeGenerator
#define DIST_LOWER_LIMIT 0
#define DIST_UPPER_LIMIT 100

#include <memory>
#include <queue>
#pragma once

class QueueTape {
  public:
    QueueTape() : returned(false), dummies(0), seriesCount(0), last(Record()) {}
    bool returned;
    bool inSeries;
    int seriesCount;
    int dummies;
    Record last;
    std::deque<Record> vals;
    void Push(Record rec) {
        if (this->last.IsEmpty() || this->last > rec) // TODO: nierownosci
            this->seriesCount++;

        this->last = rec;
        vals.push_back(rec);
    };

    void Clear() {
        last = Record();
        this->inSeries = true;
        this->seriesCount = (0);
        vals = std::deque<Record>();
    }
    void restore() { returned = true; }
    Record GetNext() {
        if (returned) {
            returned = false;
            return last;
        }
        auto res = vals.front();
        vals.pop_front();
        this->inSeries = last.IsEmpty() || last <= res; // TODO: nierownosci
        this->last = res;
        return res;
    }
    Record GetCurrent() { return vals.front(); }
    bool HasNext() { return returned || !vals.empty(); };
    friend std::ostream &operator<<(std::ostream &os, const QueueTape &tp) {
        bool sorted = true;
        int i = 0;
        for (i = 0; i < tp.vals.size(); i++) {
       //     std::cout << i + 1 << "\t" << tp.vals[i];
            if (i + 1 < tp.vals.size() && tp.vals[i] > tp.vals[i + 1]) {
                sorted = false;
      //         std::cout << "SERIES BREAK" << std::endl;
            }
        }
        if (sorted)
            std::cout << "SORTED: " << i << " elems" << std::endl;
        else
            std::cout << "NON SORTED: " << i << " elems" << std::endl;
        return os;
    }
};

class PMSTest {
    std::vector<std::shared_ptr<QueueTape>> QueueTapes;
    std::string filename;

  public:
    PMSTest::PMSTest(std::shared_ptr<QueueTape> inputQueueTape) {}
    PMSTest::PMSTest(std::string _filename) : filename(_filename) {

        QueueTapes.push_back(std::make_shared<QueueTape>());
        std::default_random_engine gen(clock()); // clock()
        std::uniform_real_distribution<double> dist(DIST_LOWER_LIMIT,
                                                    DIST_UPPER_LIMIT);
        std::uniform_int_distribution<int> distInt(1, 1);
        auto generator = [&]() { return dist(gen); };
        auto generatorInt = [&]() { return distInt(gen); };
        for (int i = 0; i < 8*(1<<10); i++) {
            std::vector<double> gen;
            gen.push_back(generator());
            Record rec(gen);
            QueueTapes[0]->Push(rec);
        }
        QueueTapes.push_back(std::make_shared<QueueTape>());
        QueueTapes.push_back(std::make_shared<QueueTape>());
    }
    int PMSTest::Distribute() {
        int phases = 0;

        //    QueueTapes[0]->ChangeMode(READONLY);
        QueueTapes[1]->Clear();
        QueueTapes[2]->Clear();
        std::shared_ptr<QueueTape> src = QueueTapes[0], dest = QueueTapes[1];
        long long fib = 1, fibOld = 0, fibCopy = fib;
        while (src->HasNext()) {
            Record rec = src->GetNext();
            if (!src->inSeries &&
                dest->seriesCount == fib) { // TODO: ogarnac to
                dest = dest == QueueTapes[1] ? QueueTapes[2] : QueueTapes[1];
                fibCopy = fib;
                fib = fib + fibOld;
                fibOld = fibCopy;
                phases++;
            }
            dest->Push(rec);
        }
        dest->dummies = fib - dest->seriesCount;
        return phases;
    }
    void PMSTest::Print(int which = -1) {
        std::cout << "PRINTING QueueTapeS:" << std::endl;
        if (which == -1) {
            for (int i = 0; i < QueueTapes.size(); i++) {
                std::cout << "==================== QueueTape" << std::endl;
                std::cout << *QueueTapes[i] << std::endl;
            }
        } else {
            std::cout << "==================== QueueTape" << std::endl;
            std::cout << *QueueTapes[which] << std::endl;
        }
    }
    void PMSTest::Merge() {
        QueueTapes[0]->Clear();

        std::shared_ptr<QueueTape> QueueTape1 = QueueTapes[1],
                                   QueueTape2 = QueueTapes[2];
        Record record1, record2;
        // TODO: tu sie psuje
        // QueueTape1->ChangeMode(READONLY);
        // QueueTape2->ChangeMode(READONLY);

        while (QueueTape1->HasNext() && QueueTape2->HasNext()) {
            if (QueueTape1->dummies > 0) {
                record1 = Record();
                QueueTape1->dummies--;
            } else {
                record1 = QueueTape1->GetNext();
            }
            if (QueueTape2->dummies > 0) {
                record2 = Record();
                QueueTape2->dummies--;
            } else {
                record2 = QueueTape2->GetNext();
            }

            auto GetMinimum = [&]() {
                Record minimum;
                if (record1.IsEmpty() && record2.IsEmpty()) {
                    return Record();
                }
                if (record1.IsEmpty() ||
                    !record2.IsEmpty() && record1 > record2) {
                    minimum = record2;
                    if (QueueTape2->HasNext()) {
                        record2 = QueueTape2->GetNext();
                        if (!QueueTape2->inSeries) {
                            record2 = Record();
                            QueueTape2->restore();
                        }
                    } else
                        record2 = Record();
                } else {
                    minimum = record1;
                    if (QueueTape1->HasNext()) {
                        record1 = QueueTape1->GetNext();
                        if (!QueueTape1->inSeries) {
                            record1 = Record();
                            QueueTape1->restore();
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
                QueueTapes[0]->Push(min);
            }
        }
        Print();
        /*
        std::cout << std::endl << "NEW PHASE" << std::endl;
        std::cout << "RECORD1: " << record1 << std::endl;
        std::cout << "RECORD2: " << record2 << std::endl;
        */
        //    QueueTapes[0]->ChangeMode(READONLY);

        if (QueueTapes[1]->HasNext())
            std::swap(QueueTapes[0], QueueTapes[2]);
        else
            std::swap(QueueTapes[0], QueueTapes[1]);
    }
    void PMSTest::Sort() {
//        Print(0);
        int phases = Distribute();
        //    QueueTapes[1]->ChangeMode(READONLY);
        //    QueueTapes[2]->ChangeMode(READONLY);
//        Print();
        for (int i = 0; i < phases; i++)
            Merge();
    }
    PMSTest::~PMSTest() {}
};