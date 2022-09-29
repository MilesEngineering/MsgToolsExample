all:: .prerequisites.log

include embedded/ucplatform/mk/include.mk
# codegen.mk will run the code generator for all the default languages
include $(MK_DIR)/codegen.mk

SUBDIRS := embedded

# Run a shell script only once, and mark when it's been run so it doesn't get
# run again until it changes.  Use /bin/bash so that we can use pipefail to stop
# execution when anything in the command fails, not just the return value of tee.
.prerequisites.log: SHELL := /bin/bash
.prerequisites.log: install_prerequisites.sh
	@set -o pipefail ; ./$<  2>&1 | tee $@~
	@mv $@~ $@

# subdir.mk will run Make for all dirs in the SUBDIRS variable.
include $(MK_DIR)/subdir.mk
