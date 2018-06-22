CC = g++
CFLAGS = -g -Wall
LFLAGS = -Llib -lglfw -lglad -ldl
IFLAGS = -Iinclude
OBJFLAGS = $(CFLAGS) $(IFLAGS)
EXECFLAGS = $(CFLAGS) $(LFLAGS)
SRC = $(wildcard src/*.cpp)
OBJS = $(SRC:src/%.cpp=build/%.o)
TESTSRC = $(wildcard test/src/*.cpp)
TESTOBJS = $(TESTSRC:test/src/%.cpp=test/build/%.o)
TESTEXECS = $(TESTOBJS:test/build/%.o=test/bin/%)

default: $(TESTEXECS)

$(TESTEXECS): test/bin/%: test/build/%.o $(OBJS) test/bin/
	$(CC) $(OBJS) $< -o $@ $(EXECFLAGS)

$(TESTOBJS): test/build/%.o: test/src/%.cpp test/build/
	$(CC) $(OBJFLAGS) -c $< -o $@

$(OBJS): build/%.o: src/%.cpp build/
	$(CC) $(OBJFLAGS) -c $< -o $@

bin/:
	mkdir -p bin

build/:
	mkdir -p build

test/bin/:
	mkdir -p test/bin

test/build/:
	mkdir -p test/build

.PHONY: clean

clean:
	rm -rf build/*; rm -rf bin/*; rm -rf test/build/*; rm -rf test/bin/*

rebuild: clean default
