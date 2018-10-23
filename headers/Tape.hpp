#include <vector>
#include <fstream>
#include "Record.hpp"
#pragma once

#define BLOCK_SIZE 4096
#define DIST_UPPER_LIMIT 100.
#define DIST_LOWER_LIMIT -100.
#define SEPARATOR_VALUE 0.
#define _DEBUG_ 0

class Tape
{
  std::fstream file;
  std::string filePath;
  long long diskOpCounter;
  size_t fileSize;
  long long lastTapePos;
  std::vector<double> GetNextBlock();

public:
  Tape(std::string _filePath);
  ~Tape();
  void OpenStream();
  void CloseStream();
  std::vector<Record> RecordBlockRead();
  void GenerateTape(int);
};
