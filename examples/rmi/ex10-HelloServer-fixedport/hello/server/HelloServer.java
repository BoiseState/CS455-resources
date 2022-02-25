package hello.server;

import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

/**
 * An implementation of the Hello remote interface using RMI and fixed port for clients
 * to connect with. This makes it easier to get through a firewall.
 */
public class HelloServer implements Hello
{
    private static final long serialVersionUID = 8510538827054962873L;
    private String name;
    private static int registryPort = 1099;

    public HelloServer(String s, int port) throws RemoteException {
        UnicastRemoteObject.exportObject(this, port);
        name = s;
        System.out.println("HelloServer: " + name + " server created");
    }
    

    public String sayHello() throws RemoteException {
        return "Hello World!";
    }


    public static void main(String args[]) {
        if (args.length != 2) {
            System.err.println("Usage: java -Djava.security.policy=mysecurity.policy "
                    + "hello.server.HelloServer <serverport> <registryPort>");
            System.exit(1);
        }
        int port = Integer.parseInt(args[0]);
        registryPort = Integer.parseInt(args[1]);

        try {
            // Create and install a security manager
            System.setSecurityManager(new SecurityManager());

            Registry registry = LocateRegistry.getRegistry(registryPort);

            HelloServer obj = new HelloServer("//HelloServer", port);
            registry.rebind("HelloServer", obj);

            System.out.println("HelloServer: bound in registry");
        } catch (RemoteException e){
            System.out.println("HelloServer: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
