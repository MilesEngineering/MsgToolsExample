ALL_BUILD_SPECS := Linux same70 Python
BUILD_SPEC := Linux

# switch this from ENABLE_DEBUG_SEGGER_RTT_STRINGS to ENABLE_DEBUG_SEGGER_RTT once we have PC-side support for that.
FLAGS += -DENABLE_DEBUG_SEGGER_RTT_STRINGS -DENABLE_DEBUG_MSGS -D_ASSERT_ENABLE_

SRC = \
    ucplatform/src/hw_init.cpp          \
    ucplatform/src/tick.cpp             \
    ucplatform/src/config_settings.cpp  \
    ucplatform/src/debug_server.cpp     \
    ucplatform/src/debug_printf.cpp     \
    ucplatform/drivers/samx7x/serial_client.cpp       \
    ucplatform/libs/rtt/RTT/SEGGER_RTT.c        \
    ucplatform/libs/rtt/RTT/SEGGER_RTT_printf.c \
    ucplatform/drivers/samx7x/fault_handlers.cpp      \
    ucplatform/drivers/samx7x/hardfault_handler.cpp   \
    ucplatform/src/freertos_fault_handlers.cpp \
    ucplatform/src/base_can_client.cpp  \
    ucplatform/drivers/$(BUILD_SPEC_DIRNAME)/can_client.cpp

ifeq (Python,$(BUILD_SPEC))
TARGET=example_app$(shell python3-config --extension-suffix)
else
SRC += main.cpp
endif

INCLUDE_SAM_USB_SUPPORT := true

include ../ucplatform/mk/common.mk
