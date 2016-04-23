package rmisslex1fixedport;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.RMIClientSocketFactory;
import java.rmi.server.RMIServerSocketFactory;
import java.rmi.server.RemoteServer;
import java.rmi.server.ServerNotActiveException;
import java.rmi.server.UnicastRemoteObject;
import javax.rmi.ssl.SslRMIClientSocketFactory;
import javax.rmi.ssl.SslRMIServerSocketFactory;

import java.util.Date;

public class DateServerImpl implements DateServer
{
    public static final int PREFERRED_PORT = 5005;
    public static final int REGISTRY_PORT = 5111;
    
    public DateServerImpl() throws RemoteException { }

    public void bind(String name)
    {
	try {
	    Registry registry = LocateRegistry.getRegistry(DateServerImpl.REGISTRY_PORT);
	    RMIClientSocketFactory rmiClientSocketFactory = new FixedPortSslRMIClientSocketFactory();
	    RMIServerSocketFactory rmiServerSocketFactory = new FixedPortSslRMIServerSocketFactory();
	    DateServer ccAuth = (DateServer) UnicastRemoteObject.exportObject(this, 0, rmiClientSocketFactory,
	            	rmiServerSocketFactory);
	    
	    registry.rebind(name, ccAuth);
	    System.out.println(name + " bound in registry");
	} catch (Exception e) {
	    e.printStackTrace();
	    System.out.println("Exception occurred: " + e);
	}
    }

    public Date getDate() throws RemoteException
    {
	try {
	    System.out.println("Connect from: " + RemoteServer.getClientHost());
	} catch (ServerNotActiveException e) {
	    System.err.println(e);
	}
	return new java.util.Date();
    }

    public static void main(String args[])
    {
	System.out.println("Setting System Properties....");
	System.setProperty("javax.net.ssl.keyStore", "rmisslex1timeout/resources/Server_Keystore");
	// Warning: change to match your password! Also the password should be
	// stored encrypted in a file outside the program.
	System.setProperty("javax.net.ssl.keyStorePassword", "test123");
	System.setProperty("java.security.policy", "rmisslex1timeout/resources/mysecurity.policy");
	try {
	    DateServerImpl server = new DateServerImpl();
	    server.bind("DateServerImpl");
	} catch (Throwable th) {
	    th.printStackTrace();
	    System.out.println("Exception occurred: " + th);
	}
    }
}

class FixedPortSslRMIServerSocketFactory extends SslRMIServerSocketFactory
{
    public ServerSocket createServerSocket(int port) throws IOException
    {
	if (port == 0) {
	    return super.createServerSocket(DateServerImpl.PREFERRED_PORT);
	}
	return super.createServerSocket(port);
    }
}



class FixedPortSslRMIClientSocketFactory extends SslRMIClientSocketFactory
{
    public Socket createSocket(String host, int port) throws IOException
    {
	Socket s = super.createSocket(host, DateServerImpl.PREFERRED_PORT);
	return s;
    }  
}
