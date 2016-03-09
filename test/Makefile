CXX = g++
CXXFLAGS = -O0 -Wall -std=c++11 -pedantic -DDEBUG -g
SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .cpp .o .h
.PHONY: all

PROG = SA_test time_test file_test

all: $(PROG)

SA_test:   SA_test.o ../src/SA.o
time_test: time_test.o
file_test: file_test.o

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PROG):
	$(CXX) $(CXXFLAGS) $^ -o $@


