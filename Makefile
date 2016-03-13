CXX = g++
CXXFLAGS = -O0 -Wall -ansi -pedantic -DDEBUG -g

SRC = src
LIB = lib
TEST = test
BIN = bin

.PHONY: all test clean

all:
	@cd $(SRC) && $(MAKE)
	@cd $(TEST) && $(MAKE)

test: all
	@cd $(TEST) && $(MAKE)

clean:
	@echo "Cleaning..."
	@rm -r *.o
	@echo "done!"
	@echo ""

