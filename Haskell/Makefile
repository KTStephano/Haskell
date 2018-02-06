FLAGS=-std=c++14 -O2
GCC=g++

EXE=Haskell++
SOURCE=$(wildcard ./Examples/*.cpp) $(wildcard ./Source/*.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCE))

%.o: %.cpp
	@echo Compiling $<
	@$(GCC) $(FLAGS) -c $<

all: $(OBJECTS)
	$(GCC) $(FLAGS) *.o -o $(EXE)

clean:
	rm -f *.o $(EXE)
