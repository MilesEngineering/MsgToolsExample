main_all : check_objectdir $(PROJECT_TARGETS)
#	@echo "make: built targets of `pwd`"
	@printf "\033[0;34m<==== $(CURDIR) built\033[0m\n"

build_all :
	@echo "building target default for ALL build-specs";\
	for spec in _dummy_ $(ALL_BUILD_SPECS); do\
		if [ "$$spec" = "_dummy_" ]; then\
			continue ;\
		fi;\
		echo " ";\
		echo "building all for build-spec '$$spec'";\
		"$(MAKE)" $(MFLAGS) BUILD_SPEC=$$spec DEBUG_MODE=$(DEBUG_MODE) TRACE=$(TRACE) || exit;\
	done

clean_all :
	@echo "building target clean for ALL build-specs";\
	for spec in _dummy_ $(ALL_BUILD_SPECS); do\
		if [ "$$spec" = "_dummy_" ]; then\
			continue ;\
		fi;\
		echo " ";\
		echo "building clean for build-spec '$$spec'";\
		"$(MAKE)" $(MFLAGS) BUILD_SPEC=$$spec DEBUG_MODE=$(DEBUG_MODE) TRACE=$(TRACE) clean || exit;\
	done

clobber_all :
	rm -rf obj
