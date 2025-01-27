package square.client;

import square.server.Square;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class SquareClient {
	public static void main(String[] args)
	{
		if (args.length < 4) {
			System.err.println("Usage: java SquareClient <hostname> <value> <#calls> <registry-port>");
			System.exit(1);
		}
		String host = args[0];
		int value = Integer.parseInt(args[1]);
		int count = Integer.parseInt(args[2]);
		int registryPort = Integer.parseInt(args[3]);

		try {
			Registry registry = LocateRegistry.getRegistry(host, registryPort);
			Square stub = (Square) registry.lookup("SquareServer");

			int result = 0;
			for (int i = 0; i < count; i++) {
				result = stub.square(value);
				if (i % 1000 == 0) System.out.printf("Call# %d result = %d\r ", i, result);
			}
			System.out.println(result);

		} catch (Exception e) {
			System.err.println("Client exception: " + e.toString());
			e.printStackTrace();
		}
	}
}
