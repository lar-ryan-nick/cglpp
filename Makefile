ifdef COMSPEC
CC = cl.exe
LINKER = link.exe
CFLAGS = /c /EHsc /nologo /Wall /I include /Fo
LFLAGS = /LIBPATH:lib /OUT:
CLEARER = del
MDFLAGS =
BUILDDIR = build\ 
BINDIR = bin\ 
TESTBUILDDIR = test\build\ 
TESTBINDIR = test\bin\ 
else
CC = g++
LINKER = g++
CFLAGS = -c -g -Wall -Iinclude -std=c++11 -o 
CLEARER = rm -rf
LFLAGS = -Llib -lglfw -lglad -lGL -lX11 -lpthread -lXrandr -lXi -ldl -o 
MDFLAGS = -p
BUILDDIR = build/
BINDIR = bin/
TESTBUILDDIR = test/build/
TESTBINDIR = test/bin/
.PHONY: clean
	
endif

SRC = $(wildcard src/*.cpp)
OBJS = $(SRC:src/%.cpp=build/%.o)
TESTSRC = $(wildcard test/src/*.cpp)
TESTOBJS = $(TESTSRC:test/src/%.cpp=test/build/%.o)
TESTEXECS = $(TESTOBJS:test/build/%.o=test/bin/%)

default: $(TESTEXECS)

$(TESTEXECS): $(TESTBINDIR)%: $(TESTBUILDDIR)%.o $(OBJS) $(TESTBINDIR)
	$(LINKER) $(OBJS) $< $(LFLAGS)$@

$(TESTOBJS): $(TESTBUILDDIR)%.o: test/src/%.cpp $(TESTBUILDDIR)
	$(CC) $< $(CFLAGS)$@

$(OBJS): $(BUILDDIR)%.o: src/%.cpp $(BUILDIR)
	$(CC) $< $(CFLAGS)$@

$(BINDIR):
	mkdir $(MDFLAGS) $(BINDIR)

$(BUILDIR):
	mkdir $(MDFLAGS) $(BUILDDIR)

$(TESTBINDIR):
	mkdir $(MDFLAGS) $(TESTBINDIR)

$(TESTBUILDDIR):
	mkdir $(MDFLAGS) $(TESTBUILDDIR)

clean:
	$(CLEARER) build/*
	$(CLEARER) bin/*
	$(CLEARER) test/build/*
	$(CLEARER) test/bin/*

rebuild: clean default
