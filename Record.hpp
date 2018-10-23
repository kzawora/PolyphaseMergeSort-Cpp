#include <vector>
#pragma once
class Record
{
  public:
    std::vector<double> values;
    long long recordPosition;
    Record();
    Record(std::vector<double> x);
    bool operator<(const Record &lhs) const;
    bool operator>(const Record &lhs) const;
    bool operator<=(const Record &lhs) const;
    bool operator>=(const Record &lhs) const;
    friend std::ostream& operator<<(std::ostream& os, const Record& rec);
    void Add(double val);
    size_t Size();
};
