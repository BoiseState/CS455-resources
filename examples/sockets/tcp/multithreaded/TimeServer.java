package tcp.multithreaded;

import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.net.InetAddress;

/**
 * A multithreaded time server.
 * 
 * @author amit
 */
public class TimeServer
{
    private int port;
    private ServerSocket ss;

    /**
     * Creates a server socket that listens on the specified port number. 
     * @param port The port number for the server.
     */
    public TimeServer(int port)
    {
	try {
	    ss = new ServerSocket(port);
	    System.out.println("TimeServer up and running on port " + port);
	} catch (IOException e) {
	    System.err.println(e);
	}
    }

    /**
     * The main server method that accepts connections and starts off a new thread
     * to handle each accepted connection.
     */
    public void runServer()
    {
	Socket client;
	try {
	    while (true) {
		client = ss.accept();
		System.out.println("Received connect from " + client.getInetAddress().getHostName() 
			+ " [ " + client.getInetAddress().getHostAddress() + " ] ");
		new ServerConnection(client).start();
	    }
	} catch (IOException e) {
	    System.err.println(e);
	}
    }

    public static void main(String args[])
    {
	if (args.length < 1) {
	    System.err.println("Usage: java TimeServer <port>");
	    System.exit(1);
	}
	TimeServer server = new TimeServer(Integer.parseInt(args[0]));
	server.runServer();
    }
}

/**
 * Handles one connection in a separate thread.
 */
class ServerConnection extends Thread
{
    private Socket client;

    ServerConnection(Socket client) throws SocketException
    {
	this.client = client;
	setPriority(NORM_PRIORITY - 1);
	System.out.println("Created thread " + this.getName());
    }

    public void run()
    {
	try {
	    OutputStream out = client.getOutputStream();
	    InputStream in = client.getInputStream();
	    ObjectOutputStream oout = new ObjectOutputStream(out);
	    
	    oout.writeObject(new java.util.Date());
	    oout.flush();

	    Thread.sleep(10000);
	    client.close();
	} catch (InterruptedException e) {
	    System.out.println(e);
	} catch (IOException e) {
	    System.out.println("I/O error " + e);
	}
    }
}
