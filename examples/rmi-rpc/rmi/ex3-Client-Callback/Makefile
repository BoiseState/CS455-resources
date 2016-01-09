#---------------------------------------------------------------
# Using this Makefile
#
#	To compile your java source (and generate documentation)
#
#	make 
#
#	To clean up your directory (e.g. before submission)
#
#	make clean
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

# To satisfy the rule named compile, we must have the  following 
# class files (with date no later than the source .java files).
# We also must have satisfied the rule named doc.

all: compile 

compile: compile-server compile-client

compile-server: callback/server/MyServer.class callback/server/Request.class callback/server/Server.class callback/server/StringEnumeration.class callback/server/StringEnumerator.class callback/server/WorkListener.class callback/server/WorkRequest.class


rmi: compile
	rmic MyServer
	cd callback/server/; cp Server.class MyServer_Stub.class Request.class WorkRequest.class StringEnumerator.class StringEnumeration.class ../client

compile-client: callback/client/MyCalculation.class callback/client/MyClient.class 
	cd callback/client/; cp MyCalculation.class ../server


clean:
		rm --force  callback/server/*.class callback/client/*.class


