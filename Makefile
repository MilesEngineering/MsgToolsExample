all:: check cpp c dart python java js swift matlab html

.PHONY: python cpp c java js swift matlab html check

python:
	msgparser messages obj/Python python

cpp:
	msgparser messages obj/Cpp cpp

c:
	msgparser messages obj/C c

dart:
	msgparser messages obj/Dart/lib dart

java:
	msgparser messages obj/Java java

js:
	msgparser messages obj/Javascript javascript

swift:
	msgparser messages obj/Swift swift

matlab:
	msgparser messages obj/Matlab/+Messages matlab

html:
	msgparser messages obj/Html html
	@find obj/Html -type d -print0 | xargs -n 1 -0 cp html/bootstrap.min.css

check: $(DIGEST)

clean clobber::
	rm -rf obj

MSG_FILES := $(shell cd messages && find * -iname \*.yaml)

$(DIGEST): $(addprefix messages/,$(MSG_FILES)) $(CG_DIR)check.py
	$(call colorecho,Checking message validity)
	msgcheck $(call CYGPATH,$(DIGEST)) messages
