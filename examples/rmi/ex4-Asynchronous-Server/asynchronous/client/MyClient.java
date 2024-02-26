package asynchronous.client;

import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

import asynchronous.server.Server;
import asynchronous.server.StringEnumerationRequest;
import asynchronous.server.WorkListener;
import asynchronous.server.WorkRequest;
import asynchronous.client.MyClient;

/**
 * Shows a client making multiple asynchronous calls to a server. However the
 * client now needs to keep track of the number of outstanding asynchronous
 * calls so that it doesn't quit too early. This requires synchronization.
 */
public class MyClient extends java.rmi.server.UnicastRemoteObject implements WorkListener
{
    private Object allDone;
    private static final long serialVersionUID = -6314695118464643327L;
    private int count = 0;
    private boolean requestsSent = false;

    public static void main(String[] args) throws RemoteException {
        if (args.length != 2) {
            System.err.println("Usage: java MyClient <server host> <registry port>");
            System.exit(1);
        }
        String hostName = args[0];
        int registryPort = Integer.parseInt(args[1]);
        new MyClient(hostName, registryPort);
    }


    public MyClient(String host, int registryPort) throws RemoteException {
        allDone = new Object();
        try {
            Registry registry = LocateRegistry.getRegistry(host, registryPort);
            Server server = (Server) registry.lookup("AsyncServer");
            //We can also use the following style
            //Server server = (Server) Naming.lookup("rmi:" + registryPort + "//" + host + "/AsyncServer");

            System.out.println(server.getDate());
            System.out.println(server.getDate());
            System.out.println(server.execute(new MyCalculation(2)));
            StringEnumerationRequest se = server.getList();
            while (se.hasMoreItems())
                System.out.println(se.nextItem());

            incCount();
            server.asyncExecute(new MyCalculation(10000), this);
            incCount();
            server.asyncExecute(new MyCalculation(1000), this);
            incCount();
            server.asyncExecute(new MyCalculation(10), this);
            incCount();
            server.asyncExecute(new MyCalculation(50), this);
            requestsSent = true;

            try {
                synchronized (allDone) {
                    allDone.wait();
                }
            } catch (InterruptedException e) {
                System.err.println(e);
            }
            System.out.println(server.getDate());
            System.exit(0);

        } catch (RemoteException e) {
            System.out.println(e);
        } catch (NotBoundException e) {
            System.out.println(e);
        }
    }


    public synchronized void incCount() {
        count++;
    }


    public synchronized void decCount() {
        count--;
    }


    public synchronized void workCompleted(WorkRequest request, Object result) throws RemoteException {
        System.out.println("MyClient: recvd notification from server");
        System.out.flush();
        int[] results = (int[]) result;
        System.out.println("MyClient: recvd array of length " + results.length);
        decCount();
        synchronized (allDone) {
            if (count == 0 && requestsSent) allDone.notify();
        }
    }
}
