#include <vector>
#pragma once
class Record {
    std::vector<double> values;

  public:
    Record();
    Record(std::vector<double> x);

    void Add(double val);
    size_t Size();
    std::vector<double> GetValues();

    bool operator<(const Record &) const;
    bool operator>(const Record &) const;
    bool operator<=(const Record &) const;
    bool operator>=(const Record &) const;
    friend std::ostream &operator<<(std::ostream &, const Record &);
};
