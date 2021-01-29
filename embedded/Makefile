ALL_BUILD_SPECS := Linux sam
BUILD_SPEC := sam
SAM_VARIANT := same70

TARGET := example_app
SRC := \
    main.cpp                     \
    ucplatform/tick.cpp          \
    ucplatform/debug_server.cpp  \
    ucplatform/debug_printf.cpp  \
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

include ucplatform/mk/common.mk
