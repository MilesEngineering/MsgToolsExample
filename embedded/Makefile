ALL_BUILD_SPECS := Linux sam
BUILD_SPEC := sam
SAM_VARIANT := same70

TARGET := example_app
SRC := \
    main.cpp \
    console.cpp

ifneq (,$(findstring sam,$(BUILD_SPEC)))
SRC += \
    sam/serial_client.cpp \
    sam/can_client.cpp

# uncomment this to enable USB CDC
include mk/sam.usb.mk

endif

include mk/common.mk
