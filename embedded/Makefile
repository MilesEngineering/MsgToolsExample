ALL_BUILD_SPECS := Linux sam
BUILD_SPEC := sam
SAM_VARIANT := same70

TARGET := example_app
SRC := \
    main.cpp                     \
    freertos_fault_handlers.cpp

ifneq (,$(findstring sam,$(BUILD_SPEC)))
SRC += \
    sam/console.cpp           \
    sam/serial_client.cpp     \
    sam/can_client.cpp        \
    sam/fault_handlers.cpp 

# uncomment this to enable USB CDC
include mk/sam.usb.mk

endif

include mk/common.mk
