CC = g++
CFLAGS = -std=c++17 -c -Wall
LDFLAGS =
BIN = bin
#DIRS = bin
SRCS = $(shell find src -maxdepth 2 -name "*.cpp" -printf %f'\n')
SRCDIR = src
INCLUDE = -I include
OBJS = $(SRCS:.cpp=.o)
EXEC = tracer
MKDIR_P = mkdir -p
RM = rm -f
VPATH = src ../include
MODULES = glm

build: $(EXEC)

clean:
	@echo=off $(RM) ./$(BIN)/*;

.PHONY: build clean

all: @echo=off $(EXEC)

$(EXEC): $(addprefix $(BIN)/, $(OBJS))
	$(CC) $(LDFLAGS) $^ $(INCLUDE) -o ./$(BIN)/$@ 

$(BIN)/%.o: %.cpp | directories
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@ 

directories:
	@echo=off $(MKDIR_P) $(BIN)

# all: 
# 	g++ --std=c++11 main.cpp tga.cpp -I ./ -o softgl