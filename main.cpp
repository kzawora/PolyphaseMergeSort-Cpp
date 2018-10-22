#include <iostream>
#include "Tape.cpp"
int main() {
	const size_t tapeSize = 20, setCount = 3;
	Tape tape1("test.bin");
	tape1.GenerateTape(tapeSize, setCount);

	std::vector<double> numbers;
	tape1.OpenStream();
	for (int i = 0; i < tapeSize; i++)
		numbers.push_back(tape1.GetNext());
	tape1.CloseStream();

	return 0;
}