FILES = src/main.cpp
OUTPUT_FILE = bin/somtd
COMPILER_FLAGS = -std=c++11
LINKER_FLAGS = -lSDL2
CC = g++

all : 
	@mkdir -p bin
	$(MAKE) $(OUTPUT_FILE)

$(OUTPUT_FILE):
	$(CC) $(FILES) $(COMPILER_FLAGS) $(LIKER_FLAGS) -o $(OUTPUT_FILE)
