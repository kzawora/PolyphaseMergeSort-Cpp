#include <vector>
#pragma once
class Record
{
public:
  std::vector<double> values;
  long long recordPosition;
  Record();
  Record(std::vector<double> x);
  bool operator<(const Record &) const;
  bool operator>(const Record &) const;
  bool operator<=(const Record &) const;
  bool operator>=(const Record &) const;
  friend std::ostream &operator<<(std::ostream &, const Record &);
  void Add(double val);
  size_t Size();
};
