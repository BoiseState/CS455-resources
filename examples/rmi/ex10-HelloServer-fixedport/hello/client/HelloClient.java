package hello.client;

import hello.server.Hello;

import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

/**
 * A simple RMI client that emulates Hello World.
 *
 */
public class HelloClient
{
    public static void main(String[] args) {

        if (args.length < 2) {
            System.err.println("Usage: java HelloClient <host> <registry-port>");
            System.exit(1);
        }
        String host = null;
        int registryPort;
        host = args[0];
        registryPort = Integer.parseInt(args[1]);

        
        try {
            Registry registry = LocateRegistry.getRegistry(host, registryPort);
            Hello stub = (Hello) registry.lookup("HelloServer");

            String response = stub.sayHello();
            System.out.println("response: " + response);
        } catch (RemoteException | NotBoundException e) {
            System.err.println("HelloClient: remote exception " + e);
        }
        
    }
}
