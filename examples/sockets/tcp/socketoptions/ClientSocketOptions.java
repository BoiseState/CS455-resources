package tcp.socketoptions;

import java.io.*;
import java.net.*;

public class ClientSocketOptions
{
    private static final int EXIT_FAILURE = 1;

    public static void main(String args[])
    {
	String serverHost = "localhost";
	if (args.length == 1) {
	    serverHost = args[0];
	} else {
	    System.out.println("Usage: java ClientSocketOptions [<server host>]");
	    System.exit(EXIT_FAILURE);
	}
	try {
	    Socket s = new Socket(serverHost, 5005);
	    System.out.println("Socket s");
	    System.out.println("SO_TIMEOUT = " + s.getSoTimeout());
	    System.out.println("Receive Buffer Size = " + s.getReceiveBufferSize());
	    System.out.println("Send Buffer Size = " + s.getSendBufferSize());
	    System.out.println("TCP_NODELAY  = " + s.getTcpNoDelay());
	    System.out.println("SO_LINGER  = " + s.getSoLinger());
	    System.out.println("TCP_KEEPALIVE  = " + s.getKeepAlive());
	    System.out.println("SO_REUSEADDR  = " + s.getReuseAddress());
	    System.out.println("Using local adress:port" + s.getLocalSocketAddress());
	    s.close();
	} catch (IOException e) {
	    System.out.println(e);
	}
    }
}
