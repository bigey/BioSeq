CXX = g++
CXXFLAGS = -O0 -Wall -ansi -pedantic -DDEBUG -g
SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .cpp .o
.PHONY: all

SRC = Dna.cpp  GenericSeq.cpp  SA.cpp  SeqIO.cpp  SimpleSA.cpp
OBJECTS = $(SRC:.cpp=.o)

.PHONY: all

all: $(OBJECTS)

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@
