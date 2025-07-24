all:: .prerequisites.log

include embedded/ucplatform/mk/include.mk
# codegen.mk will run the code generator for all the default languages
include $(MK_DIR)/codegen.mk

SUBDIRS := embedded

# subdir.mk will run Make for all dirs in the SUBDIRS variable.
include $(MK_DIR)/subdir.mk
