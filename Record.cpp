#include <iostream>
#include <vector>
#include <algorithm>
class Record {
public:
	Record(std::vector<double> x) : values(x) {}
	std::vector<double> values;
	friend bool operator< (const Record& lhs, const Record& rhs) {
		std::vector<double> lCopy = lhs.values, rCopy = rhs.values;
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
	friend bool operator> (const Record& lhs, const Record& rhs) { return rhs < lhs; }
	friend bool operator<=(const Record& lhs, const Record& rhs) { return !(lhs > rhs); }
	friend bool operator>=(const Record& lhs, const Record& rhs) { return !(lhs < rhs); }
};