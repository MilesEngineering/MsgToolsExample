ALL_BUILD_SPECS := Linux same70 Python riscv same54p20
BUILD_SPEC := Linux

# switch this from ENABLE_DEBUG_SEGGER_RTT_STRINGS to ENABLE_DEBUG_SEGGER_RTT once we have PC-side support for that.
FLAGS += -DENABLE_DEBUG_SEGGER_RTT_STRINGS -DENABLE_DEBUG_MSGS -D_ASSERT_ENABLE_

SRC = \
    ucplatform/src/tick.cpp             \
    ucplatform/src/config_settings.cpp  \
    ucplatform/src/debug_server.cpp     \
    ucplatform/src/debug_printf.cpp     \
    ucplatform/libs/rtt/RTT/SEGGER_RTT.c        \
    ucplatform/libs/rtt/RTT/SEGGER_RTT_printf.c \
    ucplatform/drivers/cortexM/fault_handlers.cpp     \
    ucplatform/drivers/cortexM/hardfault_handler.cpp  \
    ucplatform/src/freertos_fault_handlers.cpp

ifeq (Python,$(BUILD_SPEC))
TARGET=example_app$(shell python3-config --extension-suffix)
else
SRC += main.cpp
endif

INCLUDE_SAM_USB_SUPPORT := true

include ../ucplatform/mk/common.mk
