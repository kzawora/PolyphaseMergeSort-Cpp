#include <iostream>
#include "Tape.cpp"
#include "Record.cpp"
#pragma once

int main()
{
	const size_t size = 4 * (1 << 10); // 4Mi
	Tape tape1("test.bin");
	tape1.GenerateTape(size);

	tape1.OpenStream();
	std::vector<Record> recs;
	while (recs.size() < size)
	{
		auto temp = tape1.RecordBlockRead();
		recs.insert(recs.end(), temp.begin(), temp.end());
	}
	for (auto val : recs)
		std::cout << val;

	tape1.CloseStream();

	Record r1({1., 2., 3., 999999.});
	Record r2({.1, 2., 4., 6., 1., 2., 6., 1060., 2.});
	std::cout << (r1 > r2) << std::endl;

	return 0;
}
