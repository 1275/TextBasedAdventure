# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Compiler settings
CXX = g++
CXXFLAGS = -Wall -I$(INC_DIR)

# Source files
SOURCES = $(SRC_DIR)/Utilities.cpp \
          $(SRC_DIR)/Player.cpp \
          $(SRC_DIR)/Menu.cpp \
          $(SRC_DIR)/WorldVariables.cpp \
          $(SRC_DIR)/Action.cpp \
          $(SRC_DIR)/Location.cpp \
          $(SRC_DIR)/Game.cpp \
          $(SRC_DIR)/Main.cpp

# Object files (in build directory)
OBJECTS = $(BUILD_DIR)/Utilities.o \
          $(BUILD_DIR)/Player.o \
          $(BUILD_DIR)/Menu.o \
          $(BUILD_DIR)/WorldVariables.o \
          $(BUILD_DIR)/Action.o \
          $(BUILD_DIR)/Location.o \
          $(BUILD_DIR)/Game.o \
          $(BUILD_DIR)/Main.o

# Target executable
TARGET = run

# Default target
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Pattern rule for object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Dependencies
$(BUILD_DIR)/Utilities.o: $(INC_DIR)/Utilities.h $(SRC_DIR)/Utilities.cpp

$(BUILD_DIR)/Player.o: $(INC_DIR)/Player.h $(SRC_DIR)/Player.cpp

$(BUILD_DIR)/Menu.o: $(INC_DIR)/Menu.h $(SRC_DIR)/Menu.cpp

$(BUILD_DIR)/WorldVariables.o: $(INC_DIR)/WorldVariables.h $(SRC_DIR)/WorldVariables.cpp

$(BUILD_DIR)/Action.o: $(INC_DIR)/Action.h $(SRC_DIR)/Action.cpp

$(BUILD_DIR)/Location.o: $(INC_DIR)/Location.h $(INC_DIR)/Menu.h $(SRC_DIR)/Location.cpp

$(BUILD_DIR)/Game.o: $(INC_DIR)/Menu.h $(INC_DIR)/Game.h $(SRC_DIR)/Game.cpp

$(BUILD_DIR)/Main.o: $(INC_DIR)/Game.h $(SRC_DIR)/Main.cpp

push:
	git push origin master

clean:
	rm -rf $(BUILD_DIR) $(TARGET) *~ $(INC_DIR)/*~

.PHONY: all clean push