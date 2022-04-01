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
add_executable(imagelize
        test/8bit.cpp
        test/32bit.cpp
        test/64bit.cpp
        )

target_link_libraries(imagelize PRIVATE Catch2::Catch2WithMain)
target_include_directories(imagelize PRIVATE ./)

##########
# SAMPLE #
##########
add_executable(imagelize-sample sample/main.cpp)
target_include_directories(imagelize-sample PRIVATE ./ ext)