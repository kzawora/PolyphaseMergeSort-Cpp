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

// for Record
#define _DEBUG_SORT_ 1

// for TapeGenerator
#define DIST_LOWER_LIMIT -49
#define DIST_UPPER_LIMIT 59

// for Tape
#define BLOCK_SIZE 4 * (1 << 10)
#define WRITE_BLOCK_SIZE 4 * (1 << 10)

#define SEPARATOR_VALUE std::nan("")
