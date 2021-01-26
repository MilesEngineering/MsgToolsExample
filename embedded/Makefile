ALL_BUILD_SPECS := Linux sam
BUILD_SPEC := sam
SAM_VARIANT := same70

TARGET := example_app
SRC := \
    main.cpp                     \
    ucplatform/debug_printf.cpp  \
    ucplatform/freertos_fault_handlers.cpp

ifeq (sam,$(BUILD_SPEC))
SRC += \
    ucplatform/sam/console.c             \
    ucplatform/sam/serial_client.cpp     \
    ucplatform/sam/can_client.cpp        \
    ucplatform/sam/fault_handlers.cpp 

INCLUDE_SAM_USB_SUPPORT := true

endif

include ucplatform/mk/common.mk
