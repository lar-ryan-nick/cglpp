SRCDIR = src/
INCLUDEDIR = include/
BUILDDIR = build/
BINDIR = bin/
TESTSRCDIR = test/src/
TESTINCLUDEDIR = test/include/
TESTMAINDIR = test/main/
TESTBUILDDIR = test/build/
TESTBINDIR = test/bin/
LIBSRCDIR = lib/src/
LIBINCLUDEDIR = lib/include/
LIBBUILDDIR = lib/build/
LIBBINDIR = lib/bin/
GLFWDIR = glfw/
GLADDIR = glad/
FREETYPEDIR = freetype/
ASSIMPDIR = assimp/

GLFW = $(LIBBINDIR)libglfw3.a
GLAD = $(LIBBINDIR)libglad.a
FREETYPE = $(LIBBINDIR)libfreetype.a
ASSIMP = $(LIBBINDIR)libassimp.a
DEPENDENCIES = $(GLFW) $(GLAD) $(FREETYPE) $(ASSIMP) $(GLM) $(STB)
CGL = $(BINDIR)libcgl.a

SRC = $(wildcard $(SRCDIR)*.cpp)
INCLUDE = $(wildcard $(INCLUDEDIR)*.h)
OBJS = $(SRC:$(SRCDIR)%.cpp=$(BUILDDIR)%.o)
TESTSRC = $(wildcard $(TESTSRCDIR)*.cpp)
TESTINCLUDE = $(wildcard $(TESTINCLUDEDIR)*.h)
TESTMAIN = $(wildcard $(TESTMAINDIR)*.cpp)
TESTOBJS = $(TESTSRC:$(TESTSRCDIR)%.cpp=$(TESTBUILDDIR)%.o)
TESTEXECS = $(TESTMAIN:$(TESTMAINDIR)%.cpp=$(TESTBINDIR)%)

CC = gcc
CXX = g++ -std=c++11
CFLAGS = -g -Wall -I$(INCLUDEDIR) -I$(LIBINCLUDEDIR)
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
LFLAGS = -L$(LIBBINDIR) -L$(BINDIR) -lcgl -lglfw3 -lglad -lassimp -lIrrXML -lfreetype -lbz2 -lpng -lz -lpthread -ldl -framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreVideo
else
LFLAGS = -L$(LIBBINDIR) -L$(BINDIR) -lcgl -lglfw3 -lglad -lassimp -lIrrXML -lfreetype -lbz2 -lpng -lX11 -lz -lpthread -ldl
endif
LIBBER = ar rcs
CMAKE = cmake
RM = rm -rf
MKDIR = mkdir -p
CP = cp -r
RENAME = mv

.PHONY: default clean rebuild tests dependencies cleanall rebuildall
	
.PRECIOUS: $(CGL) $(DEPENDENCIES)

.SECONDARY: $(OBJS) $(TESTOBJS)

.NOTPARALLEL: rebuild rebuildall

default: $(CGL)

$(CGL): $(OBJS) | $(BINDIR)
	$(RM) $@
	$(LIBBER) $@ $(OBJS)

$(BUILDDIR)%.o: $(SRCDIR)%.cpp $(INCLUDE) dependencies | $(BUILDDIR)
	$(CXX) $(CFLAGS) -c $< -o $@

$(BINDIR):
	$(MKDIR) $(BINDIR)

$(BUILDDIR):
	$(MKDIR) $(BUILDDIR)

tests: $(TESTEXECS)

$(TESTBINDIR)%: $(TESTMAINDIR)%.cpp $(CGL) $(TESTOBJS) | $(TESTBINDIR)
	$(CXX) $(CFLAGS) $(LFLAGS) $(TESTOBJS) $< -o $@

$(TESTBUILDDIR)%.o: $(TESTSRCDIR)%.cpp $(TESTINCLUDE) $(INCLUDE) | $(TESTBUILDDIR)
	$(CXX) $(CFLAGS) -c $< -o $@

$(TESTBINDIR):
	$(MKDIR) $(TESTBINDIR)

$(TESTBUILDDIR):
	$(MKDIR) $(TESTBUILDDIR)

clean:
	$(RM) $(BUILDDIR)
	$(RM) $(BINDIR)
	$(RM) $(TESTBUILDDIR)
	$(RM) $(TESTBINDIR)

rebuild: clean default

dependencies: $(DEPENDENCIES)

$(GLAD): $(LIBSRCDIR)$(GLADDIR) | $(LIBINCLUDEDIR) $(LIBBUILDDIR) $(LIBBINDIR)
	# Copy include files over
	$(CP) $(LIBSRCDIR)$(GLADDIR)include/* $(LIBINCLUDEDIR)
	# Build static lib
	$(MKDIR) $(LIBBUILDDIR)$(GLADDIR)
	$(CC) $(CFLAGS) -c $(LIBSRCDIR)$(GLADDIR)src/glad.c -o $(LIBBUILDDIR)$(GLADDIR)glad.o
	$(RM) $@
	$(LIBBER) $@ $(LIBBUILDDIR)$(GLADDIR)*.o

$(GLFW): $(LIBSRCDIR)$(GLFWDIR)  | $(LIBINCLUDEDIR) $(LIBBUILDDIR) $(LIBBINDIR)
	# Copy over include files
	$(CP) $(LIBSRCDIR)$(GLFWDIR)include/GLFW $(LIBINCLUDEDIR)$(GLFWDIR)
	# Build static lib
	$(CMAKE) -DBUILD_SHARED_LIBS=OFF -S $(LIBSRCDIR)$(GLFWDIR) -B $(LIBBUILDDIR)$(GLFWDIR)
	$(MAKE) -j2 -C $(LIBBUILDDIR)$(GLFWDIR)
	$(CP) $(LIBBUILDDIR)$(GLFWDIR)src/libglfw3.a $@

$(ASSIMP): $(LIBSRCDIR)$(ASSIMPDIR) | $(LIBINCLUDEDIR) $(LIBBUILDDIR) $(LIBBINDIR)
	# Copy include files over
	$(CP) $(LIBSRCDIR)$(ASSIMPDIR)include/assimp $(LIBINCLUDEDIR)
	# Build static lib
	$(CMAKE) -DBUILD_SHARED_LIBS=OFF -S $(LIBSRCDIR)$(ASSIMPDIR) -B $(LIBBUILDDIR)$(ASSIMPDIR)
	$(MAKE) -j2 -C $(LIBBUILDDIR)$(ASSIMPDIR)
	$(CP) $(LIBBUILDDIR)$(ASSIMPDIR)include/assimp/* $(LIBINCLUDEDIR)$(ASSIMPDIR)
	$(CP) $(LIBBUILDDIR)$(ASSIMPDIR)contrib/irrXML/libIrrXML.a lib/bin/libIrrXML.a
	$(CP) $(LIBBUILDDIR)$(ASSIMPDIR)code/libassimp.a $@

$(FREETYPE): $(LIBSRCDIR)$(FREETYPEDIR) | $(LIBINCLUDEDIR) $(LIBBUILDDIR) $(LIBBINDIR)
	# Copy include files over
	$(CP) $(LIBSRCDIR)$(FREETYPEDIR)include/* $(LIBINCLUDEDIR)
	# Build static lib
	$(CMAKE) -DBUILD_SHARED_LIBS=OFF -S $(LIBSRCDIR)$(FREETYPEDIR) -B $(LIBBUILDDIR)$(FREETYPEDIR)
	$(MAKE) -j2 -C $(LIBBUILDDIR)$(FREETYPEDIR)
	$(CP) $(LIBBUILDDIR)$(FREETYPEDIR)libfreetype.a $@

$(LIBINCLUDEDIR):
	$(MKDIR) $(LIBINCLUDEDIR)

$(LIBBUILDDIR):
	$(MKDIR) $(LIBBUILDDIR)

$(LIBBINDIR):
	$(MKDIR) $(LIBBINDIR)

cleanall: clean
	$(RM) $(LIBINCLUDEDIR)
	$(RM) $(LIBBUILDDIR)
	$(RM) $(LIBBINDIR)

rebuildall: cleanall default
