CXX = g++
CXXFLAGS = -O0 -Wall -ansi -pedantic -DDEBUG -g
SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .cpp .o
.PHONY: all

SRC = SA_test.cpp
PROG = $(SRC:.cpp=)

SA_test.o: SA_test.cpp
SA_test:   SA_test.o ../src/SA.o

all: $(PROG)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PROG):
	$(CXX) $(CXXFLAGS) $^ -o $@
