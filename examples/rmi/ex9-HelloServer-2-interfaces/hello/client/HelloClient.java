package hello.client;

import hello.server.Hello;

import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class HelloClient {
	public static void main(String[] args)
	{
		if (args.length < 1) {
			System.err.println("Usage: java HelloClient <host> [<registry-port>]");
			System.exit(1);
		}
		String host = null;
		int registryPort = 1099;
		if (args.length == 1) {
			host = args[0];
		} else {
			host = args[0];
			registryPort = Integer.parseInt(args[1]);
		}

		try {
			Registry registry = LocateRegistry.getRegistry(host, registryPort);
			Hello stub = (Hello) registry.lookup("EnglishServer");
			String response = stub.sayHello();
			System.out.println("response: " + response);
		} catch (RemoteException e) {
			System.err.println("HelloClient exception: " + e.toString());
			e.printStackTrace();
		} catch (NotBoundException e) {
			System.err.println("HelloClient exception: " + e.toString());
			e.printStackTrace();
		}
	}
}
