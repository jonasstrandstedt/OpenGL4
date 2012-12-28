# check what OS the user is running
uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')
ifeq ($(uname_S),Linux)
	OS = Linux
endif
ifeq ($(uname_S),Darwin)
	OS = Darwin
endif
ifneq (,$(findstring MINGW,$(uname_S)))
	OS = MinGW
endif

# specify enviromnent variables
CC = g++
MV = mv
CP = cp
RM = rm
MESSAGE =

#specify dependency list for all .cpp files
OBJECTS ?= main.o 
OBJECTS += GL4-engine/src/Engine.o
OBJECTS += GL4-engine/src/VBO.o
OBJECTS += GL4-engine/src/FBO.o
OBJECTS += GL4-engine/src/ShaderManager.o

# Name the output file, if changed then the sgct_sim.sh script needs to be edited as well
OUTPUT ?= Program
ifeq ($(OS),MinGW)
	OUTPUT+=.exe
endif

# Compiler flags to the linker
FLAGS ?=

# Compiler flags for all objects
CXXFLAGS ?= 

# include paths, specified with -isystem instead of -I due to -pedantic compiler when TEST is specified
INCPATH ?= -isystem"GL4-engine/include"

# Specify what needs to be includes, OPENGL is given (but kept as option)
OPENGL=1

# uncomment or run "make SOUND=1", removes the _NOSOUND_ define.
#SOUND=1

# uncomment or run "make TEST=1", compiles gtest and adds the _TEST_ define
#TEST=1

# uncomment or run "make RELEASE=1", only if TEST is not defined, adds the -O3 optimization flag
#RELEASE=1

# check if argument OPENGL=1 is set, reguires glfw to be properly installed
ifdef OPENGL
	MESSAGE += OpenGL,
	ifeq ($(OS),Linux)
		FLAGS += -lglfw -lGL -lGLU -lGLEW
	else ifeq ($(OS),Darwin)
		FLAGS += -framework Cocoa -framework OpenGL -lglfw -lGLEW
	else ifeq ($(OS),MinGW)
		FLAGS += -lglfw 
	endif
endif


all: $(OBJECTS)
	-@echo " "
	-@echo "Linking for $(OS)"
	-@echo "Including $(MESSAGE)"
	-@echo " "
	$(CC) $(OBJECTS) $(INCPATH) $(CXXFLAGS) $(FLAGS) -o $(OUTPUT)

# removes object files but not binaries
clean:
	-@echo "Cleaning"
	-$(RM) *.o
	-$(RM) GL4-engine/src/*.o


# pattern that compiles all .o files
%.o: %.cpp
	$(CC) $(CXXFLAGS) $(INCPATH) -c $< -o $@