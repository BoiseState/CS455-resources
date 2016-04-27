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

/**
 * Shows how to restrict all RMI calls to a single port for ease in dealing with firewalls. Note that the 
 * rmiregistry has to be on a different port. It is possible to have that be on the same port as well.
 * 
 * @author amit ((modified example from Oracle Java docs)
 *
 */
public class DateServerImpl implements DateServer
{
    public static final int PREFERRED_PORT = 5005;
    public static final int REGISTRY_PORT = 5111;

    public DateServerImpl() throws RemoteException { }

    /**
     * Binds server to registry using custom socket factories for both server and client side. This
     * helps with a firewall by reducing to two ports to punch through the firewall. Otherwise RMI uses 
     * anonymous random ports.
     * 
     * @param name the registered name of the service
     */
    public void bind(String name)
    {
	try {
	    Registry registry = LocateRegistry.createRegistry(DateServerImpl.REGISTRY_PORT);
	    System.out.println("DateServerImpl: Created registry at port " + DateServerImpl.REGISTRY_PORT);
	    
	    RMIClientSocketFactory rmiClientSocketFactory = new FixedPortSslRMIClientSocketFactory();
	    RMIServerSocketFactory rmiServerSocketFactory = new FixedPortSslRMIServerSocketFactory();
	    DateServer ccAuth = (DateServer) UnicastRemoteObject.exportObject(this, 0, rmiClientSocketFactory,
	            rmiServerSocketFactory);

	    registry.rebind(name, ccAuth);
	    System.out.println("DateServerImpl: " + name + " bound in registry");
	} catch (Exception e) {
	    e.printStackTrace();
	    System.out.println("DateServerImpl: Exception occurred: " + e);
	}
    }

    /* (non-Javadoc)
     * @see rmisslex1fixedport.DateServer#getDate()
     */
    public Date getDate() throws RemoteException
    {
	try {
	    System.out.println("DateServerImpl: Connect from: " + RemoteServer.getClientHost());
	} catch (ServerNotActiveException e) {
	    System.err.println(e);
	}
	return new java.util.Date();
    }

    public static void main(String args[])
    {
	System.out.println("DateServerImpl: Setting System Properties....");
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
	    System.out.println("DateServerImpl: Exception occurred: " + th);
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
    private static final long serialVersionUID = 3367830980062260938L;
    public Socket createSocket(String host, int port) throws IOException
    {
	Socket s = super.createSocket(host, DateServerImpl.PREFERRED_PORT);
	return s;
    }
}
