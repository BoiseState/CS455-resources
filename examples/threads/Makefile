
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


all: Account.class Consumer.class InterruptTest.class MaxThreads.class\
	 PC.class PingPong.class Producer.class ThreadRunnableExample.class\
	 SharedQueue.class SynchronizedAccount.class SynchronizedPingPong.class TestAccount.class\
	 ThreadExample.class ThreadGroupExample.class ProcessExample.class MaxPrcoesses.class\
	 ExecutorExample.class
		
clean:
	/bin/rm -f *.class
