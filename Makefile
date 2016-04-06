CXX = g++
CXXFLAGS = -O4 -Wall -ansi -pedantic -DDEBUG -g
SHELL = /bin/sh

PREFIX := $(pwd)
SRC = src
LIB = lib
TEST = test
BIN = $(CURDIR)/bin

export CXX CXXFLAGS SHELL SRC LIB TEST BIN

.PHONY: all test install clean


all:
	@cd $(SRC)  && $(MAKE)
	@cd $(TEST) && $(MAKE)

test: all
	@cd $(TEST) && $(MAKE)

install: all
	@echo "install to " $(BIN)
	@mkdir -p $(BIN)
	@cd $(SRC) && $(MAKE) install

clean:
	@echo "Cleaning..."
	@cd $(SRC)  && $(MAKE) clean
	@cd $(TEST) && $(MAKE) clean
	@echo "All done!"
