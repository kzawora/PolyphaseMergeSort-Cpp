cmake_minimum_required(VERSION 2.8)
file(GLOB SBDProjekt1_SRC
    "*.hpp"
    "*.cpp"
)
add_executable(SBDProjekt1 ${SBDProjekt1_SRC})
