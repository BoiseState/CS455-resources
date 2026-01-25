package tcp.socketoptions;

import java.io.IOException;
import java.net.Socket;

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
		    
		    System.out.println("ClientSocketOptions: Socket s");
		    System.out.println("ClientSocketOptions: SO_TIMEOUT = " + s.getSoTimeout());
		    System.out.println("ClientSocketOptions: Receive Buffer Size = " + s.getReceiveBufferSize());
		    System.out.println("ClientSocketOptions: Send Buffer Size = " + s.getSendBufferSize());
		    
		    System.out.println("ClientSocketOptions: TCP_NODELAY  = " + s.getTcpNoDelay());
		    System.out.println("ClientSocketOptions: SO_LINGER  = " + s.getSoLinger());
		    System.out.println("ClientSocketOptions: TCP_KEEPALIVE  = " + s.getKeepAlive());
		    System.out.println("ClientSocketOptions: SO_REUSEADDR  = " + s.getReuseAddress());
		    
		    System.out.println("ClientSocketOptions: Using local adress:port" + s.getLocalSocketAddress());
		    s.close();
		} catch (IOException e) {
		    System.out.println(e);
		}
    }
}
