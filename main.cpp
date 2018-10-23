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
	std::vector<std::vector<Record>> recs;
	do
		recs.push_back(tape1.RecordBlockRead());
	while (recs.back().size() != 0);
	recs.pop_back();

	int counter = 0;
	for (auto record : recs)
	{
		counter++;
		long long size = 0;
		for (auto val : record)
			size += (val.values.size() + 1) * sizeof(double);
		std::cout << "==================================== BLOCK: " << counter << ", RECORDS: " << record.size() << " (" << size << " bytes) "
				  << " ====================================" << std::endl;
		for (auto val : record)
			std::cout << val;
	}
	tape1.CloseStream();
	/*
	Record r1({1., 2., 3., 999999.});
	Record r2({.1, 2., 4., 6., 1., 2., 6., 1060., 2.});
	std::cout << (r1 > r2) << std::endl;
*/
	return 0;
}
