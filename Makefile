all:: check cpp c dart python java js swift matlab html

.PHONY: python cpp c java js swift matlab html check

python:
	msgparser messages obj/CodeGenerator/Python python

cpp:
	msgparser messages obj/CodeGenerator/Cpp cpp

c:
	msgparser messages obj/CodeGenerator/C c

dart:
	msgparser messages obj/CodeGenerator/Dart/lib dart

java:
	msgparser messages obj/CodeGenerator/Java java

js:
	msgparser messages obj/CodeGenerator/Javascript javascript

swift:
	msgparser messages obj/CodeGenerator/Swift swift

matlab:
	msgparser messages obj/CodeGenerator/Matlab/+Messages matlab

html:
	msgparser messages obj/CodeGenerator/Html html
	@find obj/CodeGenerator/Html -type d -print0 | xargs -n 1 -0 cp html/bootstrap.min.css

check: $(DIGEST)

clean clobber::
	rm -rf obj

MSG_FILES := $(shell cd messages && find * -iname \*.yaml)

$(DIGEST): $(addprefix messages/,$(MSG_FILES)) $(CG_DIR)check.py
	$(call colorecho,Checking message validity)
	msgcheck $(call CYGPATH,$(DIGEST)) messages
