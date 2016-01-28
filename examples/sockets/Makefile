
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

JFLAGS= -cp .

# Recognize files with .class and .java extensions
.SUFFIXES: .class .java

# This is a rule to convert a file with .java extension
# into a file with a .class extension. The macro $< just
# supplies the name of the file (without the extension)
# that invoked this rule.

.java.class:
	javac $(JFLAGS) $<


all:  tcp udp tinyhttpd largerhttpd 

udp: udp/UdpClient1.class udp/UdpServer1.class

tinyhttpd: tinyhttpd/TinyHttpd.class tinyhttpd/Client.class

largerhttpd: largerhttpd/LargerHttpd.class 

tcp: tcp.client tcp.singlethreaded tcp.multithreaded tcp.socketoptions tcp.objectserver

tcp.client: tcp/client/DateAtHost.class

tcp.singlethreaded: tcp/singlethreaded/TimeServer.class tcp/singlethreaded/TimeClient.class

tcp.multithreaded: tcp/multithreaded/TimeServer.class tcp/multithreaded/TimeClient.class

tcp.socketoptions: tcp/socketoptions/ServerSocketOptions.class tcp/socketoptions/ClientSocketOptions.class

tcp.objectserver: tcp/objectserver/Server.class tcp/objectserver/Client.class
	

clean:
	/bin/rm -f *.class
