package hello.server;

import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

/**
 * An implementation of the Hello remote interface.
 */
public class HelloServer extends UnicastRemoteObject implements Hello {
	private static final long serialVersionUID = 8510538827054962873L;
	private String name;
	private static int registryPort = 1099;

	public HelloServer(String s) throws RemoteException {
		super();
		name = s;
		System.out.println("HelloServer: " + name + " server created");
	}


	public String sayHello() throws RemoteException
	{
		return "Hello World!";
	}


	public static void main(String args[])
	{
		if (args.length == 0) {
			System.err.println("Usage: java hello.server.HelloServer <registryPort>");
			System.exit(1);
		}
		registryPort = Integer.parseInt(args[0]);
		try {
			Registry registry = LocateRegistry.getRegistry(registryPort);

			HelloServer obj = new HelloServer("//HelloServer");
			registry.rebind("HelloServer", obj);

			System.out.println("HelloServer: bound in registry");
		} catch (RemoteException e) {
			System.out.println("HelloServer: " + e.getMessage());
			e.printStackTrace();
		}
	}
}
