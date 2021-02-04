TOPTARGETS := build_all clean_all clobber_all

SUBDIRS := example_app can_gateway

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	@$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(SUBDIRS)
