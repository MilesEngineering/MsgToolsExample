ALL_BUILD_SPECS := Linux same70 Python
BUILD_SPEC := Linux

# switch this from ENABLE_DEBUG_SEGGER_RTT_STRINGS to ENABLE_DEBUG_SEGGER_RTT once we have PC-side support for that.
FLAGS += -DENABLE_DEBUG_SEGGER_RTT_STRINGS -DENABLE_DEBUG_MSGS -D_ASSERT_ENABLE_

SRC = \
    ucplatform/tick.cpp             \
    ucplatform/debug_server.cpp     \
    ucplatform/debug_printf.cpp     \
    ucplatform/rtt/RTT/SEGGER_RTT.c        \
    ucplatform/rtt/RTT/SEGGER_RTT_printf.c \
    ucplatform/freertos_fault_handlers.cpp

ifeq (Python,$(BUILD_SPEC))
TARGET=example_app$(shell python3-config --extension-suffix)
else
SRC += \
    main.cpp                        \
    ucplatform/hw_init.cpp          \
    ucplatform/base_can_client.cpp  \
    ucplatform/$(BUILD_SPEC_DIRNAME)/can_client.cpp

ifneq (Linux,$(BUILD_SPEC))
SRC += \
    ucplatform/sam/serial_client.cpp     \
    ucplatform/sam/fault_handlers.cpp 

INCLUDE_SAM_USB_SUPPORT := true
endif

endif

include ../ucplatform/mk/common.mk
