#include "Record.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#pragma once

Record::Record() {}
Record::Record(std::vector<double> x) : values(x) {}
bool Record::operator<(const Record &rhs) const {
    std::vector<double> lCopy = this->values, rCopy = rhs.values;
    auto descendingComparator = [](double x, double y) { return x > y; };
    std::sort(lCopy.begin(), lCopy.end(), descendingComparator);
    std::sort(rCopy.begin(), rCopy.end(), descendingComparator);
    size_t range = std::min(lCopy.size(), rCopy.size());
    for (int i = 0; i < range; i++) {
        if (rCopy[i] > lCopy[i])
            return true;
        else if (rCopy[i] < lCopy[i])
            return false;
    }
    return false;
}
bool Record::operator>(const Record &rhs) const { return rhs < *this; }
bool Record::operator<=(const Record &rhs) const { return !(*this > rhs); }
bool Record::operator>=(const Record &rhs) const { return !(*this < rhs); }

void Record::Add(double val) { this->values.push_back(val); }
size_t Record::Size() { return this->values.size(); }

std::ostream &operator<<(std::ostream &os, const Record &dt) {
    os << "RECORD (size: " << dt.values.size() << ",\trange: "
       << (dt.recordPosition - dt.values.size()) * sizeof(double) << " - "
       << (dt.recordPosition) * sizeof(double) << " bytes)\t";
    for (auto val : dt.values)
        os << val << " ";
    os << std::endl;
    return os;
}
std::vector<double> Record::GetValues() { return this->values; };
