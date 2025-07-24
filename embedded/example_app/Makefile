ALL_BUILD_SPECS := Linux same70 Python
BUILD_SPEC := Linux

# switch this from ENABLE_DEBUG_SEGGER_RTT_STRINGS to ENABLE_DEBUG_SEGGER_RTT once we have PC-side support for that.
FLAGS += -DENABLE_DEBUG_SEGGER_RTT_STRINGS -DENABLE_DEBUG_MSGS -D_ASSERT_ENABLE_

SRC = \
    ucplatform/hw_init.cpp          \
    ucplatform/tick.cpp             \
    ucplatform/config_settings.cpp  \
    ucplatform/debug_server.cpp     \
    ucplatform/debug_printf.cpp     \
    ucplatform/sam/serial_client.cpp       \
    ucplatform/rtt/RTT/SEGGER_RTT.c        \
    ucplatform/rtt/RTT/SEGGER_RTT_printf.c \
    ucplatform/sam/fault_handlers.cpp      \
    ucplatform/sam/hardfault_handler.cpp   \
    ucplatform/freertos_fault_handlers.cpp \
    ucplatform/base_can_client.cpp  \
    ucplatform/$(BUILD_SPEC_DIRNAME)/can_client.cpp

ifeq (Python,$(BUILD_SPEC))
TARGET=example_app$(shell python3-config --extension-suffix)
else
SRC += main.cpp
endif

INCLUDE_SAM_USB_SUPPORT := true

include ../ucplatform/mk/common.mk
