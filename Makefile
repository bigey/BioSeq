CXX = g++
CXXFLAGS = -O0 -Wall -ansi -pedantic -DDEBUG -g
SHELL = /bin/sh

SRC = src
LIB = lib
TEST = test
BIN = bin

export CXX CXXFLAGS SHELL SRC LIB TEST BIN

.PHONY: all test clean


all:
	@cd $(SRC)  && $(MAKE)
	@cd $(TEST) && $(MAKE)

test: all
	@cd $(TEST) && $(MAKE)

clean:
	@echo "Cleaning..."
	@cd $(SRC)  && $(MAKE) clean
	@cd $(TEST) && $(MAKE) clean
	@echo "All done!"
