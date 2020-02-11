#!make****************************Copyright (c)********************************
#                                  GlobalLogic
#
# @author Kristijan Golub - kristijan.golub@globallogic.com
#
# @date 2019-12-20
# 
# @brief APPE module make file
#
# @version 0.1
#
# @section REVISION HISTORY
#  - KG 2019-12-20 Initial implementation 
#
#*****************************************************************************/

# Compiler settings
CC = gcc
CXX = g++
LINK = g++
LINKFLAGS = -pthread
CFLAGS = -g -O0 -Wall -Werror -Wextra 
CXXFLAGS = -g -O0 -Wall -Werror -Wextra 

# Input deffinitions 
SOURCE_DIR := App
BUILD_DIR := Output
FRAMEWORK_DIR := Framework
SRC_C := $(wildcard $(SOURCE_DIR)/*.c) $(wildcard $(FRAMEWORK_DIR)/*.c)
SRC_CPP := $(wildcard $(FRAMEWORK_DIR)/*.cpp)
SRC_FILES_C := $(shell ls -R | grep "\.c$$")
SRC_FILES_CPP := $(shell ls -R | grep "\.cpp$$")

# Build output deffinitions
OBJ_C = $(SRC_FILES_C:%.c=$(BUILD_DIR)/%.o)
OBJ_CPP = $(SRC_FILES_CPP:%.cpp=$(BUILD_DIR)/%.o)
OBJ = $(OBJ_C) $(OBJ_CPP)
DEPS = $(OBJ:%.o= %.d)
EXEC = ACS

# Command macros
MOVE = mv
DEL = rm

all: $(EXEC)

$(EXEC): $(OBJ)
	$(LINK) $(CXXFLAGS) $^ -o $@ $(LINKFLAGS)

$(OBJ_C): $(SRC_C) 
	$(CC) $(CFLAGS) -MMD -MP -MT"$@" -c $^
	$(MOVE) *.o $(BUILD_DIR)/
	$(MOVE) *.d $(BUILD_DIR)/

$(OBJ_CPP): $(SRC_CPP)
	$(CXX) $(CXXFLAGS) -MMD -MP -MT"$@" -c $^
	$(MOVE) *.o $(BUILD_DIR)/
	$(MOVE) *.d $(BUILD_DIR)/

#-include $(DEPS)

clean:
	$(DEL) $(OBJ) $(EXEC) $(DEPS)