package square.server;

import java.rmi.*;
import java.rmi.registry.*;
import java.rmi.server.UnicastRemoteObject;

public class SquareServer extends UnicastRemoteObject implements Square {
	private static final long serialVersionUID = 6510598667198579812L;
	private String name;
	private static int registryPort = 1099;

	public SquareServer(String s) throws RemoteException {
		super();
		name = s;
	}


	public int square(int arg) throws RemoteException
	{
		return arg * arg;
	}


	public static void main(String args[])
	{
		if (args.length == 0) {
			System.err.println("Usage: java square.server.SquareServer <registryPort> &\n");
			System.exit(1);
		}
		registryPort = Integer.parseInt(args[0]);

		try {
			Registry registry = LocateRegistry.getRegistry(registryPort);

			SquareServer obj = new SquareServer("//SquareServer");
			registry.rebind("SquareServer", obj);
			System.out.println("SquareServer bound in registry");

		} catch (Exception e) {
			System.out.println("SquareServer err: " + e.getMessage());
			e.printStackTrace();
		}
	}
}
