# CXX = g++
# CXXFLAGS = -O0 -Wall -ansi -pedantic -DDEBUG -g
# SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .cpp .o .h
.PHONY: all clean

PROG = GenericSeq_test SeqIO_fastq_test SuffixArray_test SeqIO_test FindMatch_test

all: $(PROG)

GenericSeq_test: GenericSeq_test.o ../src/GenericSeq.o
SeqIO_fastq_test: SeqIO_fastq_test.o ../src/SeqIO_b.o ../src/GenericSeq.o
SuffixArray_test: ../src/SuffixArray.o ../src/GenericSeq.o SuffixArray_test.o
SeqIO_test: ../src/SeqIO_b.o ../src/GenericSeq.o SeqIO_test.o
FindMatch_test: ../src/FindMatch.o ../src/GenericSeq.o ../src/SuffixArray.o FindMatch_test.o 


SeqIO_fastq_test.o: SeqIO_fastq_test.cpp ../src/SeqIO_b.h ../src/GenericSeq.h
FindMatch_test.o: FindMatch_test.cpp ../src/GenericSeq.h \
 ../src/FindMatch.h ../src/GenericSeq.h ../src/SuffixArray.h
GenericSeq_test.o: GenericSeq_test.cpp ../src/GenericSeq.h
SeqIO_test.o: SeqIO_test.cpp ../src/SeqIO_b.h ../src/GenericSeq.h
SuffixArray_test.o: SuffixArray_test.cpp ../src/SuffixArray.h \
 ../src/GenericSeq.h



.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PROG):
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	@echo "Cleaning..."
	@rm -f *.o
	@echo "Done"


