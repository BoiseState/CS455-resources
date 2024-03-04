package synchronous.client;

import java.rmi.*;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

import synchronous.server.Server;
import synchronous.server.StringEnumerationRequest;
import synchronous.server.WorkRequest;

/**
 * A client that calls synchronous RMI methods on a server.
 *
 */
public class MyClient 
{
    public static void main(String[] args) throws RemoteException {
        if (args.length != 2) {
            System.err.println("Usage: java synchronous.client.MyClient <server host> <registry port>");
            System.exit(1);
        }
        String hostName = args[0];
        int registryPort = Integer.parseInt(args[1]);
        new MyClient(hostName, registryPort);
    }


    public MyClient(String host, int registryPort) throws RemoteException {
        try {
            Registry registry = LocateRegistry.getRegistry(host, registryPort);
            Server server = (Server) registry.lookup("NiftyObjectServer");

            System.out.println(server.getDate());
            System.out.println(server.execute(new MyCalculation(2)));

            StringEnumerationRequest se = server.getList();
            while (se.hasMoreItems())
                System.out.println(se.nextItem());

        } catch (RemoteException e) {
            // I/O Error or bad URL
            System.out.println("Client: remote call failed");
            System.out.println(e);
        } catch (NotBoundException e) {
            // NiftyObjectServer isn't registered
            System.out.println(e);
        }
    }
    
}


