import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

/**
 * Majority of this code is based on RMI tutorial from Oracle:
 * https://docs.oracle.com/javase/tutorial/rmi/
 * 
 * @author atulepbe
 *
 */
public class Server implements Service
{
    public static void main(String[] args) {
	if (System.getSecurityManager() == null) System.setSecurityManager(new SecurityManager());
	try {
	    // name to register in RMIRegistry
	    String name = "Service";
	    Service server = new Server();
	    Service stub = (Service) UnicastRemoteObject.exportObject(server, 0);
	    Registry registry = LocateRegistry.getRegistry();
	    registry.rebind(name, stub);
	    System.out.println("Server is bound!");
	} catch (Exception e) {
	    System.err.println("Sadface");
	    e.printStackTrace();
	}
    }


    @Override
    public Doodle execute(Doodle dood) throws RemoteException {
	System.out.println("About to change to gook!");
	dood.setDood("Gobbly-gook");
	System.out.println("Changed to gook!");
	System.out.println("Now waiting...");
	try {
	    Thread.sleep(6000);
	} catch (InterruptedException e) {
	    e.printStackTrace();
	}
	System.out.println("Gook is coming!");
	return dood;
    }
}
