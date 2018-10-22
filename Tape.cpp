#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <string>
#include <algorithm>

#define DIST_UPPER_LIMIT 100.
#define DIST_LOWER_LIMIT -100.
#define DEBUG 1

class Tape {
	std::fstream file;
	std::string filePath;
#if DEBUG == 1
	std::vector<double> numsIn;
	std::vector<double> numsOut;
#endif

public:
	Tape(std::string _filePath) : filePath(_filePath) {}
	~Tape() {}

	void OpenStream() {
		file.open(filePath, std::ios::in | std::ios::binary);
	}
	double GetNext() {
		double num = 0xFFFFFFFFFFFFFFFF;
		if (file)
			file.read(reinterpret_cast<char*>(&num), sizeof(double));
		return num;
	}

	void CloseStream() {
		file.close();
	}

	void GenerateTape(int size, int setCount) {
		file.open(filePath, std::ios::out | std::ios::binary);
		std::default_random_engine gen(clock());
		std::uniform_real_distribution<double> dist(DIST_LOWER_LIMIT, DIST_UPPER_LIMIT);
		std::uniform_int_distribution<int> distInt(1, size - 2);		// TODO: czy to na pewno jest dobrze?
		auto generator = [&]() {
			return dist(gen);
		};
		auto generatorInt = [&]() {
			return distInt(gen);
		};

		// TODO: zbiory puste?
		static std::vector<int> setIndices;
		for (int i = 0; i < setCount - 1; i++) {
			int num = generatorInt();
			while (std::find(setIndices.begin(), setIndices.end(), num) != setIndices.end())
				num = generatorInt();
#if DEBUG == 1
			setIndices.push_back(num);
#endif
		}

		for (int i = 0; i < size; i++) {
			if (std::find(setIndices.begin(), setIndices.end(), i) != setIndices.end()) {
				long long int num = 0xFFFFFFFFFFFFFFFF;
				file.write(reinterpret_cast<char*>(&num), sizeof(double));
			}
			else {
				double num = generator();
				file.write(reinterpret_cast<char*>(&num), sizeof(double));
#if DEBUG == 1
				numsIn.push_back(num);
#endif
			}
		}
		file.close();
	}
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
};
