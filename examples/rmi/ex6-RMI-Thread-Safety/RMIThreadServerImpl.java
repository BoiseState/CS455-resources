
/** Implementation of the RMIThreadServer interface for RMI.  
    Change the read() and update() functions from synchronized to
    unsynchronized to watch how you can create race conditions and
    clobber data using RMI threads.

    How it works: We have a global variable ("counter"), which is incremented
    then decremented by a call to update().  This should appear atomic to
    clients, but if there are multiple clients connecting simultaneously, each
    client potentially could read the variable at any stage when another
    thread is executing the update() method.  By synchronizing both the
    read() and update() methods, only one client is allowed to execute those
    methods at a time.  The result is that each client sees counter==0, both
    before and after executing the update() method.

    Original code by: Paul Kreiner  
	Updated by: Amit Jain
 */

import java.lang.Thread;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.rmi.registry.*;

public class RMIThreadServerImpl extends UnicastRemoteObject implements RMIThreadServer
{
    private static final long serialVersionUID = -7574367988592496327L;
    private volatile int counter = 0;
    private final int MAXCOUNT = 900000;

    public RMIThreadServerImpl() throws RemoteException {
        super();
    }


    /**
     * TEST function to practice looking at thread synchronization. This function
     * increments a counter, then decrements it back to zero. When a client tries to
     * read the counter, they should always get zero (if threads are synchronized
     * properly).
     */
    public synchronized void update() {
        int i;
        Thread p = Thread.currentThread();

        System.out.println("[server] Entering critical section: " + p.getName());
        for (i = 0; i < MAXCOUNT; i++)
            this.counter++;
        for (i = 0; i < MAXCOUNT; i++)
            this.counter--;
        System.out.println("[server] Leaving critical section: " + p.getName());

    }


    /**
     * TEST function to practice looking at thread synchronization. This allows a
     * client to read the value of the "counter" variable.
     */
    public synchronized int read() {
        return this.counter;
    }


    public static void main(String[] args) {
        try {
            int registryPort = 1099;
            RMIThreadServerImpl localObject = new RMIThreadServerImpl();
            Registry registry = LocateRegistry.getRegistry(registryPort);
            registry.rebind("RMIThreadServer", localObject);
            System.err.println("DEBUG: RMIThreadServerImpl RMI listener bound\n");
        } catch (RemoteException e) {
            System.err.println("RemoteException: " + e);
        }
    }
}
