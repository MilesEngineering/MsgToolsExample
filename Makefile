all:: check cpp c dart python java js kotlin swift matlab simulink html

.PHONY: python cpp c java js kotlin swift matlab simulink html check

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

kotlin:
	msgparser messages obj/CodeGenerator/Kotlin kotlin

swift:
	msgparser messages obj/CodeGenerator/Swift swift

matlab:
	msgparser messages obj/CodeGenerator/Matlab/+Messages matlab

simulink:
	msgparser messages obj/CodeGenerator/Simulink/+Messages simulink

html:
	msgparser messages obj/CodeGenerator/Html html
	@find obj/CodeGenerator/Html -type d -print0 | xargs -n 1 -0 cp html/css/bootstrap.min.css

check: obj/CodeGenerator/MsgDigest.txt

clean clobber::
	rm -rf obj

MSG_FILES := $(shell cd messages && find * -iname \*.yaml)

obj/CodeGenerator/MsgDigest.txt: $(addprefix messages/,$(MSG_FILES))
	$(call colorecho,Checking message validity)
	msgcheck $@ messages

save_expected_results:
	rm -rf expected/
	mv obj/ expected/
	find expected/ -type f | xargs sed -i -e 's/Created.*at.*from://'

remove_timestamps:
	find obj/ -type f | xargs sed -i -e 's/Created.*at.*from://'
