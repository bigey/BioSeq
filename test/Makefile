CXX = g++
CXXFLAGS = -O0 -Wall -ansi -pedantic -DDEBUG -g
SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .cpp .o .h
.PHONY: all

PROG = SA_test time_test fasta_test genericseq_test fastq_test SuffixArray_test 

all: $(PROG)

SA_test:    SA_test.o ../src/SA_LCP.o
time_test:  time_test.o
genericseq_test: genericseq_test.o ../src/GenericSeq.o
fasta_test:  fasta_test.o ../src/SeqIO.o ../src/GenericSeq.o
fastq_test: fastq_test.o ../src/SeqIO.o ../src/GenericSeq.o
SuffixArray_test: ../src/SuffixArray.o ../src/GenericSeq.o SuffixArray_test.o

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PROG):
	$(CXX) $(CXXFLAGS) $^ -o $@


