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

compile: encryption passwordinput providers rmisslex1 rmisslex2 securehash sslsockets

encryption: encryption/EncryptTest.class encryption/DecryptTest.class

passwordinput: passwordinput/PasswordTest.class

providers: providers/ListProviders.class

rmisslex1: rmisslex1/DateServer.class rmisslex1/DateServerImpl.class rmisslex1/DateClient.class

rmisslex2: rmisslex2/server/Square.class rmisslex2/server/SquareServer.class rmisslex2/client/SquareClient.class

securehash: securehash/SHA2Test.class

sslsockets: sslsockets/Request.class sslsockets/Client.class sslsockets/DateRequest.class\
		    sslsockets/MyCalculation.class sslsockets/Request.class sslsockets/Server.class\
		    sslsockets/WorkRequest.class


clean:
	cd encryption; make clean
	cd securehash; make clean
	cd passwordinput; make clean
	cd providers; make clean
	cd sslsockets; make clean
	cd rmisslex1; make clean
	cd rmisslex2; make clean
