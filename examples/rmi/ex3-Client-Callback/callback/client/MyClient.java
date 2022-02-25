package callback.client;

import java.rmi.*;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import callback.server.Server;
import callback.server.StringEnumerationRequest;
import callback.server.WorkListener;
import callback.server.WorkRequest;

/**
 * A client that allows call
 *
 */
public class MyClient extends java.rmi.server.UnicastRemoteObject implements WorkListener
{
    private static final long serialVersionUID = -6314695118464643327L;

    public static void main(String[] args) throws RemoteException {
        System.setSecurityManager(new SecurityManager());
        String hostName = args[0];
        int registryPort = Integer.parseInt(args[1]);
        new MyClient(hostName, registryPort);
    }


    public MyClient(String host, int registryPort) throws RemoteException {
        try {
            Registry registry = LocateRegistry.getRegistry(host, registryPort);
            Server server = (Server) registry.lookup("NiftyObjectServer");

            System.out.println(server.getDate());
            System.out.println(server.getDate());
            System.out.println(server.execute(new MyCalculation(2)));

            StringEnumerationRequest se = server.getList();
            while (se.hasMoreItems())
                System.out.println(se.nextItem());

            server.asyncExecute(new MyCalculation(100), this);
            server.asyncExecute(new MyCalculation(100), this);

            /*
             * Note that since client is also a RMI server (for callback from the server),
             * it doesn't terminate. That is why we use System.exit to force termination of
             * the client. This use isn't correct--find a better way to fix it.
             */
            System.exit(0);

        } catch (RemoteException e) {
            // I/O Error or bad URL
            System.out.println(e);
        } catch (NotBoundException e) {
            // NiftyObjectServer isn't registered
            System.out.println(e);
        }
    }


    public void workCompleted(WorkRequest request, Object result) throws RemoteException {
        System.out.println("Async work result = " + result);
        System.out.flush();
    }
    
}


