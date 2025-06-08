
# WinZigC Parser - Modular Build System
# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g -D_GNU_SOURCE
HEADER_DIR = header
APP_DIR = app
BUILD_DIR = build
TEST_DIR = winzig_test_programs

# Include directories
INCLUDES = -I$(HEADER_DIR)

# Target executable
TARGET = winzigc

# Source files (in app directory)
SOURCES = $(APP_DIR)/main.cpp $(APP_DIR)/lexer.cpp $(APP_DIR)/parser.cpp $(APP_DIR)/ast_node.cpp

# Object files (in build directory)
OBJECTS = $(SOURCES:$(APP_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build target executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(APP_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)

# Clean test output files
clean-tests:
	rm -f tree.*

# Run comprehensive tests
test: $(TARGET)
	@echo "Running comprehensive tests..."
	@passed=0; total=0; \
	for i in 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15; do \
		total=$$((total + 1)); \
		printf "Testing winzig_$$i... "; \
		if ./$(TARGET) -ast winzig_test_programs/winzig_$$i > tree.$$i 2>/dev/null && \
		   diff tree.$$i winzig_test_programs/winzig_$$i.tree >/dev/null 2>&1; then \
			echo "\033[32mPASSED\033[0m"; \
			passed=$$((passed + 1)); \
		else \
			echo "\033[31mFAILED\033[0m"; \
		fi; \
	done; \
	echo "Results: \033[32m$$passed\033[0m/\033[34m$$total\033[0m tests passed"; \
	if [ $$passed -eq $$total ]; then echo "\033[32mAll tests passed!\033[0m"; else echo "\033[31mSome tests failed.\033[0m"; exit 1; fi


# Show file structure
structure:
	@echo "Project Structure:"
	@find . -type f -name "*.cpp" -o -name "*.h" | grep -E '\.(cpp|h)$$' | head -20

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build the project (default)"
	@echo "  clean      - Remove build files and executable"
	@echo "  test       - Run all test cases"
	@echo "  clean-tests - Remove test output files"
	@echo "  structure  - Show project file structure"
	@echo "  help       - Show this help message"

.PHONY: all clean clean-tests test structure help
