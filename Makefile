# This file contains a make script for the OBD utility
#
# Author: Josh McIntyre
#

# This block defines makefile variables
WINDOWS=windows
LINUX=linux
PLATFORM=$(LINUX)

UI_FILES=src/ui/*.cpp
CORE_FILES=src/core/*.cpp
INCLUDE_CORE=src/core

BUILD_DIR=bin
BUILD_BIN=obdcmd

CC=g++
FLAGS=-std=c++11 -I$(INCLUDE_CORE)

ifeq ($(PLATFORM), $(WINDOWS))
	LIB_FLAGS=-lws2_32
else
	LIB_FLAGS=-lboost_system
endif

# This rule builds the utility
build: $(SRC_FILES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) -o $(BUILD_DIR)/$(BUILD_BIN) $(CORE_FILES) $(UI_FILES) $(LIB_FLAGS)

# This rule cleans the build directory
clean: $(BUILD_DIR)
	rm $(BUILD_DIR)/* 
	rmdir $(BUILD_DIR)
