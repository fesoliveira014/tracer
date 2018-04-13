CC = g++
CFLAGS = -std=c++17 -c -Wall
LDFLAGS =
BIN = bin
DIRS = bin
SRCS = $(shell find src -maxdepth 2 -name "*.cpp" -printf %f'\n')
SRCDIR = src
INCLUDE = -I include
OBJS = $(SRCS:.cpp=.o)
EXEC = tracer
MKDIR_P = mkdir -p
RM = rm
VPATH = src include
MODULES = glm

all: @echo=off directories tracer

tracer: $(OBJS)
	$(CC) $(LDFLAGS) $(addprefix $(BIN)/, $(OBJS)) $(INCLUDE) -o ./$(BIN)/$@ 

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDE) $< -o ./$(BIN)/$@ 

$(OBJS): | directories

directories:
	$(MKDIR_P) $(DIRS)

clean:
	@echo=off $(RM) ./$(BIN)/*;
	

# all: 
# 	g++ --std=c++11 main.cpp tga.cpp -I ./ -o softgl