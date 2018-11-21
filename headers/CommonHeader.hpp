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

// printing
#define PRINT_TAPES         1
#define PRINT_SERIES        1
// for Record
#define _DEBUG_SORT_        1

// for TapeGenerator
// for TapeGenerator
#define DIST_LOWER_LIMIT    (-100)
#define DIST_UPPER_LIMIT    (100)
#define RECORD_MIN_SIZE     (1)
#define RECORD_MAX_SIZE     (15)

// for Tape
#define BLOCK_SIZE          (4 * (1 << 10))
#define SEPARATOR_VALUE     std::nan("")
constexpr auto READONLY  =  std::ios::in | std::ios::binary;
constexpr auto WRITEONLY =  std::ios::out | std::ios::binary | std::ios::app;