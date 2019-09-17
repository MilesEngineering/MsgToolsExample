OBJ_DIR := obj
TARGET := $(OBJ_DIR)/$(TARGET)

all: ${TARGET}

.PHONY : all clean clobber

PLATFORM ?= Linux

MSG_SRC_DIR=msg
MSG_INC_DIR=msg
FREERTOS_SRC_DIR=FreeRTOS/Source
FREERTOS_INC_DIR=FreeRTOS/Source/include
PORTABLE_SRC_DIR=FreeRTOS/Source/portable/GCC/${PLATFORM}
PORTABLE_INC_DIR=FreeRTOS/Source/portable/GCC/${PLATFORM}
PORTABLE_SRC_MEM_MANG_DIR=FreeRTOS/Source/portable/MemMang
MSGTOOLS_CODEGEN_DIR=../obj/CodeGenerator/Cpp/

FLAGS = -Wall -Werror -Wextra -Wpedantic -O0 -g -MMD
CFLAGS += ${FLAGS}
CXXFLAGS += ${FLAGS}
LDFLAGS += ${FLAGS}

ifneq (,$(findstring Linux,$(PLATFORM)))
# only want -pthread for linux!
LDFLAGS += -pthread
else
# if not linux, assume an ARM
PREFIX:=arm-none-eabi-
endif

CC := ${PREFIX}gcc
CXX := ${PREFIX}g++

INCLUDE_DIRS +=	-I. -I${MSG_INC_DIR} -I${FREERTOS_INC_DIR} -I${PORTABLE_INC_DIR} -I${MSGTOOLS_CODEGEN_DIR}

VPATH += ${MSG_SRC_DIR} ${FREERTOS_SRC_DIR} ${PORTABLE_SRC_DIR} ${PORTABLE_SRC_MEM_MANG_DIR}

MSGTOOLS_SRC := message_client.cpp message.cpp  message_pool.cpp  message_queue.cpp
FREERTOS_SRC := event_groups.c list.c queue.c tasks.c timers.c port.c heap_3.c croutine.c

ALLSRC = ${SRC} ${MSGTOOLS_SRC} ${FREERTOS_SRC}

OBJS := ${ALLSRC:%.cpp=$(OBJ_DIR)/%.o}
OBJS := ${OBJS:%.c=$(OBJ_DIR)/%.o}

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	${CXX} ${INCLUDE_DIRS} ${CXXFLAGS} -c $< -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	${CC} ${INCLUDE_DIRS} ${CFLAGS} -c $< -o $@

$(TARGET): ${OBJS}
	${CXX} ${LDFLAGS} ${LD_EXE_FLAGS} -o $@ ${OBJS} ${LIBS} 

clean:
	-rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d
	-rm -f ${TARGET}

clobber:
	-rm -rf $(OBJ_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

-include $(OBJS:%.o=%.d)
