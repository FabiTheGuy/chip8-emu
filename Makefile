# Compiler related
CXX = clang++
CXX_FLAGS = -std=c++17 -Wall -Wpedantic -fsanitize=address -I../include
COMPILE = $(CXX) $(CXX_FLAGS)

# Formatter related
FORMATTER = clang-format
FORMATTER_FLAGS = --style=Google -i
FORMAT = $(FORMATTER) $(FORMATTER_FLAGS)

# File related
SOURCES = $(wildcard src/*.cpp)
HEADERS = $(wildcard include/*.hpp)
BUILD_DIR = build
TARGET = chip8emu


# Targets
format: $(SOURCES) $(HEADERS)
	$(FORMAT) $^

clean:
	rm -rf $(BUILD_DIR)/$(TARGET)

run: $(TARGET)
	./$(BUILD_DIR)/$(TARGET)

debug: $(SOURCES)
	$(COMPILE) -g -o $(BUILD_DIR)/$(TARGET)-debug $^

$(TARGET): $(SOURCES)
	$(COMPILE) -o $(BUILD_DIR)/$@ $^
