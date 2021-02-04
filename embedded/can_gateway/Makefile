ALL_BUILD_SPECS := sam
BUILD_SPEC := sam
SAM_VARIANT := same70

# output debug info as strings on RTT and stdout.
# this isn't that efficient, but doesn't require a debug dictionary.
FLAGS += -DENABLE_DEBUG_SEGGER_RTT_STRINGS -DENABLE_DEBUG_STDOUT

SRC := \
    main.cpp                     \
    ucplatform/tick.cpp          \
    ucplatform/debug_printf.cpp  \
    ucplatform/freertos_fault_handlers.cpp \
    ucplatform/rtt/RTT/SEGGER_RTT.c        \
    ucplatform/rtt/RTT/SEGGER_RTT_printf.c

SRC += \
    ucplatform/sam/console.c             \
    ucplatform/sam/serial_client.cpp     \
    ucplatform/sam/can_client.cpp        \
    ucplatform/sam/fault_handlers.cpp 

INCLUDE_SAM_USB_SUPPORT := true

include ../ucplatform/mk/common.mk
