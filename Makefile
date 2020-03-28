CC = g++ -std=c++11
LIBBER = ar rcs
CFLAGS = -g -Wall -Iinclude
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
LFLAGS = -Llib -Lbin -lcgl -lglfw3 -lLinearMath -lBulletDynamics -lBulletCollision -lglad -lassimp -lfreetype -lpng -lz -lpthread -ldl -framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreVideo
else
LFLAGS = -Llib -Lbin -lcgl -lglfw3 -lglad -lassimp -lfreetype -lpng -lX11 -lz -lpthread -ldl
endif
CLEANER = rm -rf
MKDIR = mkdir -p

SRCDIR = src/
INCLUDEDIR = include/
BUILDDIR = build/
BINDIR = bin/
TESTSRCDIR = test/src/
TESTINCLUDEDIR = test/include/
TESTMAINDIR = test/main/
TESTBUILDDIR = test/build/
TESTBINDIR = test/bin/

LIB = $(BINDIR)libcgl.a
SRC = $(wildcard $(SRCDIR)*.cpp)
INCLUDE = $(wildcard $(INCLUDEDIR)*.h)
OBJS = $(SRC:$(SRCDIR)%.cpp=$(BUILDDIR)%.o)
TESTSRC = $(wildcard $(TESTSRCDIR)*.cpp)
TESTINCLUDE = $(wildcard $(TESTINCLUDEDIR)*.h)
TESTMAIN = $(wildcard $(TESTMAINDIR)*.cpp)
TESTOBJS = $(TESTSRC:$(TESTSRCDIR)%.cpp=$(TESTBUILDDIR)%.o)
TESTEXECS = $(TESTMAIN:$(TESTMAINDIR)%.cpp=$(TESTBINDIR)%)

.PHONY: default clean rebuild tests
	
.SECONDARY: $(OBJS) $(TESTOBJS)

.NOTPARALLEL: rebuild

default: $(LIB)

$(LIB): $(OBJS) | $(BINDIR)
	$(CLEANER) $(BINDIR)*
	$(LIBBER) $@ $(OBJS)

$(BUILDDIR)%.o: $(SRCDIR)%.cpp $(INCLUDE) | $(BUILDDIR)
	$(CC) -c $(CFLAGS) $< -o $@

tests: $(TESTEXECS)

$(TESTBINDIR)%: $(TESTMAINDIR)%.cpp $(LIB) $(TESTOBJS) | $(TESTBINDIR)
	$(CC) $(CFLAGS) $(LFLAGS) $(TESTOBJS) $< -o $@

$(TESTBUILDDIR)%.o: $(TESTSRCDIR)%.cpp $(TESTINCLUDE) $(INCLUDE) | $(TESTBUILDDIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BINDIR):
	$(MKDIR) $(BINDIR)

$(BUILDDIR):
	$(MKDIR) $(BUILDDIR)

$(TESTBINDIR):
	$(MKDIR) $(TESTBINDIR)

$(TESTBUILDDIR):
	$(MKDIR) $(TESTBUILDDIR)

clean:
	$(CLEANER) $(BUILDDIR)
	$(CLEANER) $(BINDIR)
	$(CLEANER) $(TESTBUILDDIR)
	$(CLEANER) $(TESTBINDIR)

rebuild: clean default
