package sslsockets;

import java.io.EOFException;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.SocketException;

import javax.net.ssl.SSLServerSocket;
import javax.net.ssl.SSLServerSocketFactory;
import javax.net.ssl.SSLSocket;

public class Server
{
    public static void main(String argv[]) throws IOException {
	if (argv.length < 1) {
	    System.err.println("Usage: java Server <port>");
	    System.exit(1);
	}
	System.setProperty("javax.net.ssl.keyStore", "sslsockets/resources/Server_Keystore");
	System.setProperty("javax.net.ssl.keyStorePassword", "test123");
	SSLServerSocketFactory sslSrvFact = (SSLServerSocketFactory) SSLServerSocketFactory.getDefault();
	SSLServerSocket ss = (SSLServerSocket) sslSrvFact.createServerSocket(Integer.parseInt(argv[0]));
	while (true)
	    new ServerConnection((SSLSocket) ss.accept()).start();
    }
}

class ServerConnection extends Thread
{
    SSLSocket client;


    ServerConnection(SSLSocket client) throws SocketException {
	this.client = client;
	setPriority(NORM_PRIORITY - 1);
    }


    public void run() {
	try {
	    ObjectOutputStream out = new ObjectOutputStream(client.getOutputStream());
	    ObjectInputStream in = new ObjectInputStream(client.getInputStream());

	    while (true) {
		out.writeObject(processRequest(in.readObject()));
		out.flush();
	    }
	} catch (EOFException e3) { // Normal EOF
	    try {
		client.close();
	    } catch (IOException e) {
	    }
	} catch (IOException e) {
	    System.out.println("I/O error " + e); // I/O error
	} catch (ClassNotFoundException e2) {
	    System.out.println(e2); // Unknown type of request object
	}
    }


    private Object processRequest(Object request) {
	if (request instanceof DateRequest)
	    return new java.util.Date();
	else if (request instanceof WorkRequest)
	    return ((WorkRequest) request).execute();
	else
	    return null;
    }
}
