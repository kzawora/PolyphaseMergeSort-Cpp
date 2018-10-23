#include <iostream>
#include "Tape.cpp"
#include "Record.cpp"
#pragma once

int main()
{
	const size_t size = 1024;
	Tape tape1("test.bin");
	tape1.GenerateTape(size);

	tape1.OpenStream();

	auto recs = tape1.RecordBlockRead();
	for (auto val : recs)
		std::cout << val;
	recs = tape1.RecordBlockRead();
	for (auto val : recs)
		std::cout << val;

	tape1.CloseStream();

	Record r1({1., 2., 3., 999999.});
	Record r2({.1, 2., 4., 6., 1., 2., 6., 1060., 2.});
	std::cout << (r1 < r2) << std::endl;

	return 0;
}