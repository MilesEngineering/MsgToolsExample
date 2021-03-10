# Note: to build within simulink, use "mex -setup" and "mex -setup c++" from matlab to setup the C/C++ compiler

OBJ_DIR := ../obj/simulink
TARGET := $(OBJ_DIR)/msgtools.a
all: $(TARGET)

SRC := simulink_message_interface.cpp simulink_message_client.cpp message_client.cpp

FLAGS := -Wall -Werror -Wpedantic -Wextra $(OPTIMIZATION) -g -MMD $(FLAGS)

CFLAGS += ${FLAGS}
CXXFLAGS += ${FLAGS}
LDFLAGS += 

CC := ${PREFIX}gcc
CXX := ${PREFIX}g++
AR := ${PREFIX}ar

OBJS := ${SRC:%.cpp=$(OBJ_DIR)/%.o}
OBJS := ${OBJS:%.c=$(OBJ_DIR)/%.o}
DEPS := ${OBJS:%.o=%.d}

MSGTOOLS_CODEGEN_DIR ?= ../obj/CodeGenerator/

INCLUDE_DIRS += -I${MSGTOOLS_CODEGEN_DIR} -I.

$(OBJ_DIR):
	@mkdir -p $@

# rules to compile and link
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	${CXX} ${INCLUDE_DIRS} ${CXXFLAGS} -c -fPIC $< -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	${CC} ${INCLUDE_DIRS} ${CFLAGS} -c -fPIC $< -o $@

$(TARGET): ${OBJS}
	${AR} rcs ${LDFLAGS} -o $@ ${OBJS}

# clean by deleting only files we intended to build
clean::
	-rm -f $(OBJS)
	-rm -f $(DEPS)
	-rm -f ${TARGET}

# clobber by removing whole directory
clobber::
	-rm -rf $(OBJ_DIR)

-include $(DEPS)
