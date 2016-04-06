# CXX = g++
# CXXFLAGS = -O0 -Wall -ansi -pedantic -DDEBUG -g
# SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .cpp .o .h
.PHONY: all install clean

PROG = GameSetMatch
SRC = Dna.cpp  GenericSeq.cpp SuffixArray.cpp SeqIO_b.cpp \
      FindMatch.cpp GameSetMatch.cpp

OBJECTS = $(SRC:.cpp=.o)

all: $(OBJECTS) $(PROG)

GameSetMatch: GameSetMatch.o FindMatch.o \
	SeqIO_b.o GenericSeq.o SuffixArray.o

GameSetMatch.o: GameSetMatch.cpp GenericSeq.h SeqIO_b.h \
	GenericSeq.h FindMatch.h SuffixArray.h
Dna.o: Dna.cpp Dna.h GenericSeq.h
GenericSeq.o: GenericSeq.cpp GenericSeq.h
SuffixArray.o: SuffixArray.cpp SuffixArray.h GenericSeq.h
SeqIO_b.o: SeqIO_b.cpp SeqIO_b.h GenericSeq.h
FindMatch.o: FindMatch.cpp FindMatch.h GenericSeq.h SuffixArray.h

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(PROG):
	$(CXX) $(CXXFLAGS) $^ -o $@

install:
	@chmod ug+x $(PROG)
	@mv $(PROG) $(BIN)
	@echo "Done"

clean:
	@echo "Cleaning..."
	@rm -f *.o
	@echo "Done"
