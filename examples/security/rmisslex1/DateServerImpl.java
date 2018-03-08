package rmisslex1;

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
    public DateServerImpl() throws RemoteException {
    }


    public void bind(String name) {
	try {
	    RMIClientSocketFactory rmiClientSocketFactory = new SslRMIClientSocketFactory();
	    RMIServerSocketFactory rmiServerSocketFactory = new SslRMIServerSocketFactory();
	    DateServer ccAuth = (DateServer) UnicastRemoteObject.exportObject(this, 0, rmiClientSocketFactory,
	            rmiServerSocketFactory);
	    Registry registry = LocateRegistry.createRegistry(2004);
	    registry.rebind(name, ccAuth);
	    System.out.println(name + " bound in registry");
	} catch (Exception e) {
	    e.printStackTrace();
	    System.out.println("Exception occurred: " + e);
	}
    }


    public Date getDate() throws RemoteException {
	try {
	    System.out.println("Connect from: " + RemoteServer.getClientHost());
	} catch (ServerNotActiveException e) {
	    System.err.println(e);
	}
	return new java.util.Date();
    }


    public static void main(String args[]) {
	System.out.println("Setting System Properties....");
	System.setProperty("javax.net.ssl.keyStore", "rmisslex1/resources/Server_Keystore");
	// Warning: change to match your password! Also the password should be
	// stored encrypted in a file outside the program.
	System.setProperty("javax.net.ssl.keyStorePassword", "test123");
	System.setProperty("java.security.policy", "rmisslex1/resources/mysecurity.policy");
	try {
	    DateServerImpl server = new DateServerImpl();
	    server.bind("DateServerImpl");
	} catch (Throwable th) {
	    th.printStackTrace();
	    System.out.println("Exception occurred: " + th);
	}
    }
}
