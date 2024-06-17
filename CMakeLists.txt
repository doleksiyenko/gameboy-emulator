cmake_minimum_required(VERSION 3.20.0)
project(gameboy)

set(CMAKE_CXX_STANDARD 20)
include_directories(${CMAKE_SOURCE_DIR}/include)


set(SourceFiles
    src/main.cpp
    src/gameboy.cpp)

set(HeaderFiles
    include/gameboy.h)

add_executable(${PROJECT_NAME} ${SourceFiles} ${HeaderFiles}) 

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2_mixer::SDL2_mixer)