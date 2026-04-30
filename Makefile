CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-reorder -Wno-switch

TARGET = main.exe
SRC = src/main.cpp src/lexer/lexer.cpp src/lexer/reader.cpp src/utils/utils.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	.\$(TARGET)

clean:
	del /Q $(TARGET)

.PHONY: all run clean