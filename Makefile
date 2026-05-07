CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-reorder -Wno-switch

SRC = src/main.cpp \
      src/lexer/lexer.cpp \
      src/lexer/reader.cpp \
      src/utils/utils.cpp \
      src/parser/parser.cpp \
      src/parser/treeprinter.cpp

ifeq ($(OS),Windows_NT)
	TARGET = main.exe
	RUN_CMD = .\$(TARGET)
	CLEAN_CMD = del /Q $(TARGET)
else
	TARGET = main.exe
	RUN_CMD = ./$(TARGET)
	CLEAN_CMD = rm -f $(TARGET)
endif

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: $(TARGET)
	$(RUN_CMD)

clean:
	$(CLEAN_CMD)

.PHONY: all run clean