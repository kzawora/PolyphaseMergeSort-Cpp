#include "Block.hpp"
#include "PolyphaseMergeSort.hpp"
#include "Record.hpp"
#include "Tape.hpp"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#pragma once

#define _DEBUG_SORT_ 0

// for TapeGenerator
#define DIST_LOWER_LIMIT 0
#define DIST_UPPER_LIMIT 100

// for Tape.hpp

#define BLOCK_SIZE 4 * (1 << 10)
#define WRITE_BLOCK_SIZE 4 * (1 << 10)

#define SEPARATOR_VALUE std::nan("")
#define _DEBUG_ 0
