#########################
# PROJECT CONFIGURATION #
#########################
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

project(imagelize)

#########
# CATCH #
#########
Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.0-preview3
)

FetchContent_MakeAvailable(Catch2)

#########
# TESTS #
#########
add_executable(imagelize test/test.cpp)

target_link_libraries(imagelize PRIVATE Catch2::Catch2WithMain)
target_include_directories(imagelize PRIVATE ./)
