ALL_BUILD_SPECS := Linux sam
BUILD_SPEC := Linux
SAM_VARIANT := same70

# switch this from ENABLE_DEBUG_SEGGER_RTT_STRINGS to ENABLE_DEBUG_SEGGER_RTT once we have PC-side support for that.
FLAGS += -DENABLE_DEBUG_SEGGER_RTT_STRINGS -DENABLE_DEBUG_MSGS

SRC := \
    main.cpp                        \
    ucplatform/tick.cpp             \
    ucplatform/base_can_client.cpp  \
    ucplatform/debug_server.cpp     \
    ucplatform/debug_printf.cpp     \
    ucplatform/freertos_fault_handlers.cpp \
    ucplatform/rtt/RTT/SEGGER_RTT.c        \
    ucplatform/rtt/RTT/SEGGER_RTT_printf.c

ifeq (sam,$(BUILD_SPEC))
SRC += \
    ucplatform/sam/console.c             \
    ucplatform/sam/serial_client.cpp     \
    ucplatform/sam/can_client.cpp        \
    ucplatform/sam/fault_handlers.cpp 

INCLUDE_SAM_USB_SUPPORT := true

endif

include ../ucplatform/mk/common.mk
