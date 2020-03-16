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

TARGET := appeExec

# Compiler settings
CC = gcc
CXX = g++
LINK = g++
LINKFLAGS = -pthread
CFLAGS = -g -O0 -Wall -Werror -Wextra 
CXXFLAGS = -g -O0 -Wall -Werror -Wextra 

# Input definitions
SOURCE_DIR := App
FRAMEWORK_DIR := Framework

BUILD_DIR := Output

.DEFAULT_GOAL :=  $(BUILD_DIR)/$(TARGET)

# sources
SRC_C := $(wildcard $(SOURCE_DIR)/*.c) $(wildcard $(FRAMEWORK_DIR)/*.c)
SRC_CPP := $(wildcard $(FRAMEWORK_DIR)/*.cpp)

#make sure all objects are located in build directory
OBJECTS := $(addprefix $(BUILD_DIR)/,$(SRC_C) $(SRC_CPP))
#also make sure objects have .o extension
OBJECTS := $(addsuffix .o,$(OBJECTS))

#include generated dependency files to allow incremental build when only headers change
-include $(OBJECTS:%.o=%.d)

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	@echo Creating executable: $@
	@$(LINK) $(CXXFLAGS) $^ -o $@ $(LINKFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(@D)
	@echo Building: $<
	@$(CC) $(CFLAGS) -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -c "$<" -o "$@"

$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(@D)
	@echo Building: $<
	@$(CXX) $(CXXFLAGS) -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -c "$<" -o "$@"

exec: $(BUILD_DIR)/$(TARGET)
	@chmod +x ./testGenerate.sh && ./testGenerate.sh -e
	@./$(BUILD_DIR)/$(TARGET)
	@chmod +x analyseResults.sh && ./analyseResults.sh

clean:
	@echo Cleaning up.
	@rm -rf $(BUILD_DIR)/
