CXX = g++
CXXFLAGS = -O0 -Wall -ansi -pedantic -DDEBUG -g
SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .cpp .o
.PHONY: all

SRC = Dna.cpp  GenericSeq.cpp  SA.cpp SeqIO.cpp SA_LCP.cpp SuffixArray.cpp SeqIO_b.cpp
OBJECTS = $(SRC:.cpp=.o)

all: $(OBJECTS)

Dna.o: Dna.cpp Dna.h GenericSeq.h
GenericSeq.o: GenericSeq.cpp GenericSeq.h
SA.o: SA.cpp SA.h 
SeqIO.o: SeqIO.cpp SeqIO.h GenericSeq.h
SimpleSA.o: SimpleSA.cpp
SA_LCP.o: SA_LCP.cpp SA_LCP.h
SuffixArray.o: SuffixArray.cpp SuffixArray.h GenericSeq.h
SeqIO_b.o: SeqIO_b.cpp SeqIO_b.h GenericSeq.h

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@
