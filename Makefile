CXX = clang++

CXXFLAGS = -std=c++23 -Wall -Werror -Wextra
RELEASE_FLAGS = -O2
DEBUG_FLAGS = -g -O0 -fsanitize=address,undefined

BUILD_DIR = build

SINGLE_DIR = SingleThreaded
SINGLE_SRC = $(SINGLE_DIR)/SingleThreadedMap.cpp
SINGLE_TEST = $(SINGLE_DIR)/SingleThreadedTests.cpp
SINGLE_SRC_ALL = $(SINGLE_SRC) $(SINGLE_TEST)
SINGLE_HDR = $(SINGLE_DIR)/SingleThreadedMap.hpp

MULTI_DIR = MultiThreaded
MULTI_SRC = $(MULTI_DIR)/MultiThreadedMap.cpp
MULTI_TEST = $(MULTI_DIR)/MultiThreadedTests.cpp
MULTI_SRC_ALL = $(MULTI_SRC) $(MULTI_TEST)
MULTI_HDR = $(MULTI_DIR)/MultiThreadedMap.hpp

.PHONY: all clean debug_single debug_multi

all: CXXFLAGS += $(RELEASE_FLAGS)
all: $(BUILD_DIR)/single $(BUILD_DIR)/multi

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/single: $(SINGLE_SRC_ALL) $(SINGLE_HDR) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SINGLE_SRC_ALL) -o $@

$(BUILD_DIR)/multi: $(MULTI_SRC_ALL) $(MULTI_HDR) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -pthread $(MULTI_SRC_ALL) -o $@

debug_single: CXXFLAGS += $(DEBUG_FLAGS)
debug_single: $(SINGLE_SRC_ALL) $(SINGLE_HDR) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SINGLE_SRC_ALL) -o $(BUILD_DIR)/debug_single

debug_multi: CXXFLAGS += $(DEBUG_FLAGS)
debug_multi: $(MULTI_SRC_ALL) $(MULTI_HDR) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -pthread $(MULTI_SRC_ALL) -o $(BUILD_DIR)/debug_multi

clean:
	rm -rf $(BUILD_DIR)