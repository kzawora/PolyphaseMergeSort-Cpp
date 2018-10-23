#include <iostream>
#include <vector>
#include <algorithm>
#pragma once
class Record
{
  public:
	std::vector<double> values;
	long long recordPosition;
	Record() {}
	Record(std::vector<double> x) : values(x) {}
	friend bool operator<(const Record &lhs, const Record &rhs)
	{
		std::vector<double> lCopy = lhs.values, rCopy = rhs.values;
		auto descendingComparator = [](double x, double y) { return x > y; };
		std::sort(lCopy.begin(), lCopy.end(), descendingComparator);
		std::sort(rCopy.begin(), rCopy.end(), descendingComparator);
		size_t range = std::min(lCopy.size(), rCopy.size());
		for (int i = 0; i < range; i++)
		{
			if (rCopy[i] > lCopy[i])
				return true;
			else if (rCopy[i] < lCopy[i])
				return false;
		}
		return false;
	}
	friend bool operator>(const Record &lhs, const Record &rhs) { return rhs < lhs; }
	friend bool operator<=(const Record &lhs, const Record &rhs) { return !(lhs > rhs); }
	friend bool operator>=(const Record &lhs, const Record &rhs) { return !(lhs < rhs); }

	friend std::ostream &operator<<(std::ostream &os, const Record &dt)
	{
		os << "RECORD (size: " << dt.values.size() << ",\trange: " << (dt.recordPosition - dt.values.size()) * sizeof(double) << " - " << (dt.recordPosition) * sizeof(double) << " bytes)\t";
		for (auto val : dt.values)
			os << val << " ";
		os << std::endl;
		return os;
	}

	void Add(double val)
	{
		values.push_back(val);
	}
	size_t Size()
	{
		return this->values.size();
	}
};