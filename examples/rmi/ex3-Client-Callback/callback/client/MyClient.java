package callback.client;

import java.rmi.*;
import java.security.*;

import callback.server.Server;
import callback.server.StringEnumeration;
import callback.server.WorkListener;
import callback.server.WorkRequest;

public class MyClient extends java.rmi.server.UnicastRemoteObject implements WorkListener
{

    private static final long serialVersionUID = -6314695118464643327L;


    public static void main(String[] args) throws RemoteException {
	System.setSecurityManager(new SecurityManager());
	new MyClient(args[0]);
    }


    public MyClient(String host) throws RemoteException {
	try {
	    Server server = (Server) Naming.lookup("rmi://" + host + "/NiftyServer");

	    System.out.println(server.getDate());
	    System.out.println(server.getDate());
	    System.out.println(server.execute(new MyCalculation(2)));
	    StringEnumeration se = server.getList();
	    while (se.hasMoreItems())
		System.out.println(se.nextItem());
	    server.asyncExecute(new MyCalculation(100), this);
	    server.asyncExecute(new MyCalculation(100), this);

	    /*
	     * Note that since client is also a RMI server (for callbacks from
	     * the server), it doesn't terminate. That is why we use System.exit
	     * to force termination of the client. This use isn't correct--find
	     * a way to fix it.
	     */
	    System.exit(0);

	} catch (java.io.IOException e) {
	    // I/O Error or bad URL
	    System.out.println(e);
	} catch (NotBoundException e) {
	    // NiftyServer isn't registered
	    System.out.println(e);
	}
    }


    public void workCompleted(WorkRequest request, Object result) throws RemoteException {
	System.out.println("Async work result = " + result);
	System.out.flush();
    }
}

class mySecurityManager extends SecurityManager
{
    public void checkConnect(String host, int port, Object context) {
    }


    public void checkPermission(Permission perm) {
    }
}
