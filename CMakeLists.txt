cmake_minimum_required(VERSION 3.6)
project(Haskell)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES Haskell/Source.cpp Haskell/List.h Haskell/Tuple.h Haskell/Utility.h Haskell/Homework5.cpp)
add_executable(Haskell ${SOURCE_FILES})