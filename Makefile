# Executable name

EXE := clock

# Compiler flags

CXX := clang++
CPPFLAGS := -MMD
CXXFLAGS := -std=c++23 -Wall -Werror -Wextra -Wconversion -pedantic -Wfloat-equal -Wshadow -Wdouble-promotion -Wundef
OPT := -Os -DNDEBUG

# Directory structure

SRC_DIR := ./src
BUILD_DIR := ./build

# Gather all expected .cpp files

CPP := $(wildcard $(SRC_DIR)/*.cpp)

# Map .cpp files to their respective .o files

OBJS := $(CPP:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Build release executable

.PHONY: release
release: $(BUILD_DIR)/$(EXE)

# Run application

.PHONY: run
run: release
	$(BUILD_DIR)/$(EXE)

# Link .o object files

$(BUILD_DIR)/$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OPT) $^ -o $@

# Compile .cpp sources

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(OPT) -c $< -o $@

# Create build directory

$(BUILD_DIR):
	mkdir -p $@

# Delete build directory

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# Include .d dependencies

DEPS := $(CPP:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.d)

-include $(DEPS)
