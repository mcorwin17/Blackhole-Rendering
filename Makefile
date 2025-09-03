# =============================================================================
# Black Hole Raytracer - Professional Build System
# =============================================================================
# 
# This Makefile provides a comprehensive build system for the black hole
# raytracing engine with multiple optimization levels and debugging support.
#
# @author Maxwell Corwin
# @date 2025
# @version 2.0
# =============================================================================

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
OPTIMIZATION = -O3 -march=native -ffast-math
DEBUG_FLAGS = -g -O0 -DDEBUG
RELEASE_FLAGS = -O3 -DNDEBUG -fomit-frame-pointer

# Directories
SRC_DIR = .
BUILD_DIR = build
BIN_DIR = bin
OBJ_DIR = $(BUILD_DIR)/obj

# Source files
SOURCES = main.cc
OBJECTS = $(SOURCES:%.cc=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/blackhole

# Default target
all: release

# Release build (optimized)
release: CXXFLAGS += $(RELEASE_FLAGS) $(OPTIMIZATION)
release: $(TARGET)

# Debug build
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

# Profile build
profile: CXXFLAGS += -pg -O2
profile: $(TARGET)

# Create target binary
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	@echo "Linking $@..."
	$(CXX) $(OBJECTS) -o $@ $(CXXFLAGS)
	@echo "Build complete: $@"

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create necessary directories
$(BUILD_DIR) $(OBJ_DIR) $(BIN_DIR):
	@mkdir -p $@

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Clean complete"

# Install target (optional)
install: $(TARGET)
	@echo "Installing to /usr/local/bin..."
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Installation complete"

# Uninstall target
uninstall:
	@echo "Removing from /usr/local/bin..."
	sudo rm -f /usr/local/bin/blackhole
	@echo "Uninstallation complete"

# Run the program
run: $(TARGET)
	@echo "Executing black hole raytracer..."
	./$(TARGET)

# Performance test
perf: $(TARGET)
	@echo "Running performance test..."
	time ./$(TARGET)

# Memory usage analysis
memcheck: $(TARGET)
	@echo "Running memory check..."
	valgrind --tool=memcheck --leak-check=full ./$(TARGET)

# Code formatting (requires clang-format)
format:
	@echo "Formatting source code..."
	clang-format -i *.cc *.h

# Static analysis (requires cppcheck)
analyze:
	@echo "Running static analysis..."
	cppcheck --enable=all --std=c++17 *.cc *.h

# Documentation generation (requires doxygen)
docs:
	@echo "Generating documentation..."
	doxygen Doxyfile

# Package for distribution
package: clean release
	@echo "Creating distribution package..."
	tar -czf blackhole_raytracer_v2.0.tar.gz \
		--exclude='*.git*' \
		--exclude='build' \
		--exclude='bin' \
		--exclude='*.ppm' \
		--exclude='*.DS_Store' \
		.

# Help target
help:
	@echo "Black Hole Raytracer - Build System"
	@echo "=================================="
	@echo ""
	@echo "Available targets:"
	@echo "  all        - Build release version (default)"
	@echo "  release    - Build optimized release version"
	@echo "  debug      - Build with debug symbols"
	@echo "  profile    - Build with profiling support"
	@echo "  clean      - Remove build artifacts"
	@echo "  install    - Install to system (requires sudo)"
	@echo "  uninstall  - Remove from system (requires sudo)"
	@echo "  run        - Build and execute program"
	@echo "  perf       - Build and run performance test"
	@echo "  memcheck   - Run memory leak detection"
	@echo "  format     - Format source code"
	@echo "  analyze    - Run static code analysis"
	@echo "  docs       - Generate documentation"
	@echo "  package    - Create distribution package"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Environment variables:"
	@echo "  CXX        - C++ compiler (default: g++)"
	@echo "  CXXFLAGS   - Additional compiler flags"
	@echo ""

# Phony targets
.PHONY: all release debug profile clean install uninstall run perf memcheck format analyze docs package help

# Print build information
info:
	@echo "Build Configuration:"
	@echo "  Compiler:     $(CXX)"
	@echo "  C++ Standard: C++17"
	@echo "  Build Type:   $(if $(findstring debug,$@),Debug,Release)"
	@echo "  Optimization: $(if $(findstring debug,$@),Disabled,Enabled)"
	@echo "  Target:       $(TARGET)"
	@echo "  Sources:      $(SOURCES)"
	@echo "  Objects:      $(OBJECTS)"