#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <string>
#include <algorithm>

#define DIST_UPPER_LIMIT 100.
#define DIST_LOWER_LIMIT -100.
#define _DEBUG_ 1

class Tape {
	std::fstream file;
	std::string filePath;
	long long diskOpCounter;
	size_t fileSize;
#if _DEBUG_ == 1
	std::vector<double> _DEBUG_numsIn;
	std::vector<double> _DEBUG_numsOut;
#endif

public:
	Tape(std::string _filePath) : filePath(_filePath), diskOpCounter(0) {}
	~Tape() {}

	void OpenStream() {
		std::ifstream file_temp(filePath, std::ios::binary | std::ios::ate);
		this->fileSize = file_temp.tellg();
		file_temp.close();
		file.open(filePath, std::ios::in | std::ios::binary);
	}
	void CloseStream() {
		file.close();
	}

	/*
	double GetNext() {
		double num = NAN;
		if (file)
			file.read(reinterpret_cast<char*>(&num), sizeof(double));
		return num;
	}
	*/
	std::vector<double> GetNextBlock() {
		const size_t blockSize = 4096, arraySize = blockSize / sizeof(double);
		int position = file.tellg();
		double x[arraySize] = {};
		if (file && (position + blockSize <= fileSize))
			file.read(reinterpret_cast<char*>(&x), blockSize);
		else if (file)
			file.read(reinterpret_cast<char*>(&x), fileSize - position);
		diskOpCounter++;
		return std::vector<double>(x, x + sizeof x / sizeof x[0]);
	}

	void GenerateTape(int size) {
		file.open(filePath, std::ios::out | std::ios::binary);
		std::default_random_engine gen(clock());
		std::uniform_real_distribution<double> dist(DIST_LOWER_LIMIT, DIST_UPPER_LIMIT);
		std::uniform_int_distribution<int> distInt(1, 15);		// TODO: czy to na pewno jest dobrze?
		auto generator = [&]() {
			return dist(gen);
		};
		auto generatorInt = [&]() {
			return distInt(gen);
		};
		for (int i = 0; i < size; i++) {
			int setSize = generatorInt();
			;
			while (setSize--) {
				double num = generator();
				file.write(reinterpret_cast<char*>(&num), sizeof(double));
#if _DEBUG_ == 1
				_DEBUG_numsIn.push_back(num);
#endif
			}

			double nan = NAN;
			file.write(reinterpret_cast<char*>(&nan), sizeof(double));
			_DEBUG_numsIn.push_back(nan);
		}
		file.close();
	}
	/*
	void ReadFromFile() {
		file.open(filePath, std::ios::in | std::ios::binary);
		while (file) {
			double num;
			file.read(reinterpret_cast<char*>(&num), sizeof(double));
#if DEBUG == 1
			if (file) numsOut.push_back(num);
#endif
		}
		file.close();
	}
	*/

#if _DEBUG_ == 1
	bool IsVectorEqual(std::vector<double> vec) {
		return vec == this->_DEBUG_numsIn;
	}
#endif
};