
#---------------------------------------------------------------
# Using this Makefile
#
#   To compile your java source (and generate documentation)
#
#   make
#
#   To clean up your directory (e.g. before submission)
#
#   make clean
#
#---------------------------------------------------------------

JFLAGS=

# Recognize files with .class and .java extensions
.SUFFIXES: .class .java

# This is a rule to convert a file with .java extension
# into a file with a .class extension. The macro $< just
# supplies the name of the file (without the extension)
# that invoked this rule.

.java.class:
	javac $(JFLAGS) $<


all: subdirs 

subdirs:
	cd client/; make
	cd objectserver/; make
	cd socketoptions/; make
	cd singlethreaded/; make
	cd multithreaded/; make

clean:
	cd client/; make clean
	cd objectserver/; make clean
	cd socketoptions/; make clean
	cd singlethreaded/; make clean
	cd multithreaded/; make clean
	/bin/rm -f *.class
