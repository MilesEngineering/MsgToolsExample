TOPTARGETS := build_all clean_all clobber_all clean clobber

SUBDIRS := example_app can_gateway simulink_example

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	@$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(SUBDIRS)
