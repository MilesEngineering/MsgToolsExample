ALL_BUILD_SPECS := same70 Linux
BUILD_SPEC := same70

# output debug info as strings on RTT and stdout.
# this isn't that efficient, but doesn't require a debug dictionary.
FLAGS += -DENABLE_DEBUG_SEGGER_RTT_STRINGS -DENABLE_DEBUG_STDOUT

SRC = \
    main.cpp                               \
    ucplatform/hw_init.cpp                 \
    ucplatform/tick.cpp                    \
    ucplatform/base_can_client.cpp         \
    ucplatform/debug_printf.cpp            \
    ucplatform/freertos_fault_handlers.cpp \
    ucplatform/rtt/RTT/SEGGER_RTT.c        \
    ucplatform/rtt/RTT/SEGGER_RTT_printf.c \
    ucplatform/$(BUILD_SPEC_DIRNAME)/can_client.cpp

ifneq (Linux,$(BUILD_SPEC))
SRC += \
    ucplatform/sam/serial_client.cpp       \
    ucplatform/sam/fault_handlers.cpp 

INCLUDE_SAM_CAN_SUPPORT := true
INCLUDE_SAM_USB_SUPPORT := true

endif

include ../ucplatform/mk/common.mk
