CXX = g++
CXXFLAGS = -O0 -Wall -ansi -pedantic -DDEBUG -g
SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .cpp .o .h
.PHONY: all

PROG = SA_test time_test genericseq_test fastq_test SuffixArray_test SeqIO_test

all: $(PROG)

SA_test:    SA_test.o ../src/SA_LCP.o
time_test:  time_test.o
genericseq_test: genericseq_test.o ../src/GenericSeq.o
fastq_test: fastq_test.o ../src/SeqIO_b.o ../src/GenericSeq.o
SuffixArray_test: ../src/SuffixArray.o ../src/GenericSeq.o SuffixArray_test.o
SeqIO_test: ../src/SeqIO_b.o ../src/GenericSeq.o SeqIO_test.o


fasta_test.o: fasta_test.cpp ../src/SeqIO.h ../src/GenericSeq.h
fastq_test.o: fastq_test.cpp ../src/SeqIO_b.h ../src/GenericSeq.h
genericseq_test.o: genericseq_test.cpp ../src/GenericSeq.h
SA_test.o: SA_test.cpp ../src/SA_LCP.h
SeqIO_test.o: SeqIO_test.cpp ../src/SeqIO_b.h ../src/GenericSeq.h
SuffixArray_test.o: SuffixArray_test.cpp ../src/SuffixArray.h \
 ../src/GenericSeq.h
time_test.o: time_test.cpp


.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PROG):
	$(CXX) $(CXXFLAGS) $^ -o $@


