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

public class DateServerImpl implements DateServer {
	
	public final static int REGISTRY_PORT = 5000;
	private static DateServer  ccAuth;
	private static Registry registry;
	
	public DateServerImpl() throws RemoteException {
	}


	public void bind(String name)
	{
		try {
			RMIClientSocketFactory rmiClientSocketFactory = new SslRMIClientSocketFactory();
			RMIServerSocketFactory rmiServerSocketFactory = new SslRMIServerSocketFactory();
			ccAuth = (DateServer) UnicastRemoteObject.exportObject(this, 0, rmiClientSocketFactory,
			        rmiServerSocketFactory);
			registry = LocateRegistry.createRegistry(REGISTRY_PORT);
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


	public static void main(String args[]) throws RemoteException, InterruptedException
	{
		System.out.println("Setting System Properties....");
		System.setProperty("javax.net.ssl.keyStore", "rmisslex1/resources/Server_Keystore");
		// Warning: change to match your password! Also the password should be
		// stored encrypted in a file outside the program!
		System.setProperty("javax.net.ssl.keyStorePassword", "test123");
		
		DateServerImpl server = new DateServerImpl();
		server.bind("DateServerImpl");
		
		// needed to avoid the garbage collector from unexporting the server
		while (true) Thread.sleep(Integer.MAX_VALUE); 
	}
}
