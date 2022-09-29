ALL_BUILD_SPECS := Linux same70
BUILD_SPEC := same70

# switch this from ENABLE_DEBUG_SEGGER_RTT_STRINGS to ENABLE_DEBUG_SEGGER_RTT once we have PC-side support for that.
FLAGS += -DENABLE_DEBUG_SEGGER_RTT_STRINGS -DENABLE_DEBUG_MSGS

SIMULINK_MODEL_NAME := example_model
INCLUDE_SIMULINK_SUPPORT := true

SRC := \
    main.cpp                        \
    ucplatform/hw_init.cpp          \
    ucplatform/tick.cpp             \
    ucplatform/base_can_client.cpp  \
    ucplatform/debug_server.cpp     \
    ucplatform/debug_printf.cpp     \
    ucplatform/freertos_fault_handlers.cpp \
    ucplatform/rtt/RTT/SEGGER_RTT.c        \
    ucplatform/rtt/RTT/SEGGER_RTT_printf.c \

ifneq (Linux,$(BUILD_SPEC))
SRC += \
    ucplatform/sam/serial_client.cpp     \
    ucplatform/sam/can_client.cpp        \
    ucplatform/sam/fault_handlers.cpp 

INCLUDE_SAM_USB_SUPPORT := true
INCLUDE_SAM_CAN_SUPPORT := true

endif

include ../ucplatform/mk/common.mk
