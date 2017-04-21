cmake_minimum_required(VERSION 3.6)
project(Haskell)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(SOURCE_FILES Haskell/Source.cpp Haskell/HList.h Haskell/HTuple.h Haskell/Prelude.h Haskell/Prelude.cpp Haskell/Source1.cpp Haskell/Vector.h Haskell/Homework5.cpp Haskell/Integer.h)
add_executable(Haskell ${SOURCE_FILES})