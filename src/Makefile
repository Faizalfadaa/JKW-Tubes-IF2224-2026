CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-reorder -Wno-switch

TARGET = main.exe
SRC = main.cpp lexer/lexer.cpp lexer/reader.cpp utils/utils.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	.\$(TARGET)

clean:
	del /Q $(TARGET)

.PHONY: all run clean