ifdef COMSPEC
CC = cl.exe
LINKER = link.exe
CFLAGS = /c /EHsc /nologo /Wall /I include /Fo
LFLAGS = /LIBPATH:lib /OUT:
CLEANER = del
SRCDIR = "src\\"
BUILDDIR = "build\\"
BINDIR = "bin\\"
TESTSRCDIR = "test\\src\\"
TESTMAINDIR = "test\\main\\"
TESTBUILDDIR = "test\\build\\"
TESTBINDIR = "test\\bin\\"
else
CC = g++
LINKER = g++
LIBBER = ar rcs
CFLAGS = -c -g -Wall -Iinclude -std=c++11 -o 
CLEANER = rm -rf
LFLAGS = -Llib -lglfw -lglad -lpthread -ldl -lassimp -Lbin -lcgl -o 
MDFLAGS = -p
SRCDIR = src/
BUILDDIR = build/
BINDIR = bin/
TESTSRCDIR = test/src/
TESTMAINDIR = test/main/
TESTBUILDDIR = test/build/
TESTBINDIR = test/bin/
.PHONY: clean
	
endif

LIB = $(BINDIR)libcgl.a
SRC = $(wildcard $(SRCDIR)*.cpp)
OBJS = $(SRC:$(SRCDIR)%.cpp=$(BUILDDIR)%.o)
TESTSRC = $(wildcard $(TESTSRCDIR)*.cpp)
TESTOBJS = $(TESTSRC:$(TESTSRCDIR)%.cpp=$(TESTBUILDDIR)%.o)
TESTMAIN = $(wildcard $(TESTMAINDIR)*.cpp)
TESTMAINOBJS = $(TESTMAIN:$(TESTMAINDIR)%.cpp=$(TESTBUILDDIR)%.o)
TESTEXECS = $(TESTMAIN:$(TESTMAINDIR)%.cpp=$(TESTBINDIR)%)

default: $(TESTEXECS)

$(TESTEXECS): $(TESTBINDIR)%: $(TESTBUILDDIR)%.o $(TESTOBJS) $(LIB) $(TESTBINDIR)
	$(CC) $< $(TESTOBJS) $(LFLAGS)$@

$(TESTMAINOBJS): $(TESTBUILDDIR)%.o: $(TESTMAINDIR)%.cpp $(TESTBUILDDIR)
	$(CC) $< $(CFLAGS)$@

$(TESTOBJS): $(TESTBUILDDIR)%.o: $(TESTSRCDIR)%.cpp $(TESTBUILDDIR)
	$(CC) $< $(CFLAGS)$@

$(LIB): $(OBJS) $(BINDIR)
	$(LIBBER) $@ $(OBJS)

$(OBJS): $(BUILDDIR)%.o: $(SRCDIR)%.cpp $(BUILDIR)
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
	$(CLEANER) $(BUILDDIR)
	$(CLEANER) $(BINDIR)
	$(CLEANER) $(TESTBUILDDIR)
	$(CLEANER) $(TESTBINDIR)

rebuild: clean default
