ALL_BUILD_SPECS := same70 Linux riscv same54p20 
BUILD_SPEC := same70

# output debug info as strings on RTT and stdout.
# this isn't that efficient, but doesn't require a debug dictionary.
#FLAGS += -DENABLE_DEBUG_SEGGER_RTT_STRINGS 
FLAGS += -DENABLE_DEBUG_STDOUT -D_ASSERT_ENABLE_

SRC = \
    main.cpp                               \
    ucplatform/src/tick.cpp                    \
    ucplatform/src/debug_printf.cpp            \
    ucplatform/src/freertos_fault_handlers.cpp \
    ucplatform/libs/rtt/RTT/SEGGER_RTT.c        \
    ucplatform/libs/rtt/RTT/SEGGER_RTT_printf.c \

ifneq (Linux,$(BUILD_SPEC))
SRC += \
    ucplatform/drivers/cortexM/fault_handlers.cpp     \
    ucplatform/drivers/cortexM/hardfault_handler.cpp   \

INCLUDE_SAM_USB_SUPPORT := true

endif

include ../ucplatform/mk/common.mk
