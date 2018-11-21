cmake_minimum_required(VERSION 3.12)
project(SBDProjekt1)

set(CMAKE_CXX_STANDARD 14)

include_directories(headers)

add_executable(SBDProjekt1
        headers/Block.hpp
        headers/CommonHeader.hpp
        headers/PolyphaseMergeSort.hpp
        headers/Record.hpp
        headers/Tape.hpp
        sources/Block.cpp
        sources/PolyphaseMergeSort.cpp
        sources/Record.cpp
        sources/Tape.cpp
        helpers.cpp
        main.cpp
        )
