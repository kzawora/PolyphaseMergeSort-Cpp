#include "CommonHeader.hpp"
#include <utility>

Record::Record() = default;

Record::Record(std::vector<double> x) : values(std::move(x)) {}

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

void Record::Push(double val) {
    this->values.push_back(val);
#if _DEBUG_SORT_ == 1
    auto descendingComparator = [](double x, double y) { return x > y; };
    std::sort(values.begin(), values.end(), descendingComparator);
#endif
}
size_t Record::Size() { return this->values.size(); }

std::ostream &operator<<(std::ostream &os, const Record &dt) {
    os << "RECORD (size: " << dt.values.size() << ")\t";
    for (auto val : dt.values)
        os << val << " ";
    os << std::endl;
    return os;
}
std::vector<double> Record::GetValues() { return this->values; };
bool Record::operator==(const Record &rhs) const {
    return this->values == rhs.values;
};
bool Record::operator!=(const Record &rhs) const {
    return this->values != rhs.values;
};
bool Record::IsEmpty() { return this->values.empty(); }