cmake_minimum_required(VERSION 2.8)
file(GLOB SBDProjekt1_SRC
    "*.hpp"
    "*.cpp"
	"sources/*.hpp"
    "sources/*.cpp"
)

include_directories(${PROJECT_SOURCE_DIR}/headers)
add_executable(SBDProjekt1 ${SBDProjekt1_SRC})
