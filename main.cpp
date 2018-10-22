#include <iostream>
#include "Tape.cpp"
#include "Record.cpp"
int main() {
	const size_t size = 1024;
	Tape tape1("test.bin");
	tape1.GenerateTape(size);

	tape1.OpenStream();
	std::vector<double> numbers = tape1.GetNextBlock();
	std::vector<double> numbers2 = tape1.GetNextBlock();
	std::vector<double> numbers3 = tape1.GetNextBlock();
	std::vector<double> numbers4 = tape1.GetNextBlock();

	numbers.insert(numbers.end(), numbers2.begin(), numbers2.end());
	numbers.insert(numbers.end(), numbers3.begin(), numbers3.end());
	numbers.insert(numbers.end(), numbers4.begin(), numbers4.end());

	tape1.CloseStream();

	std::cout << tape1._DEBUG_IsBlockEqual(numbers, 2048) << std::endl;

	Record r1({ 1.,2.,3., 999999. });
	Record r2({ .1,2.,4., 6., 1., 2., 6., 1060., 2. });
	std::cout << (r1 < r2) << std::endl;

	return 0;
}