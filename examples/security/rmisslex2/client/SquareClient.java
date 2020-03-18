
package rmisslex2.client;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

import rmisslex2.server.Square;

public class SquareClient
{

    private SquareClient() {
    }


    public static void main(String[] args) {
	if (args.length < 4) {
	    System.err.println("Usage: java SquareClient <host> <value> <count> <registryPort>");
	    System.exit(1);
	}
	String host = null;
	int value;
	int count;
	int registryPort = 1099; //default value, but we will use our own port

	host = args[0];
	value = Integer.parseInt(args[1]);
	count = Integer.parseInt(args[2]);
	registryPort = Integer.parseInt(args[3]);

	try {
	    System.setProperty("javax.net.ssl.trustStore", "rmisslex2/resources/Client_Truststore");
	    System.setProperty("javax.net.ssl.trustStorePassword", "test123");
	    System.setProperty("java.security.policy", "rmisslex2/resources/mysecurity.policy");

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
