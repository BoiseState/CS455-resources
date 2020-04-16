import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

/**
 * The entire point of this program is to show how parameters are passed in RMI.
 * 
 * @author atulepbe
 * @author amit
 *
 */
public class Client
{
    private final static String CLIENT_STUB_INTERFACE = "Service";
    private final static String HOST = "localhost";
    private static Doodle doodie = new Doodle();

    public Client() {
    }
    
    private void startThread() {
        new Thread(new PrintingThread()).start();
    }


    public static void main(String args[]) {
	if (System.getSecurityManager() == null) System.setSecurityManager(new SecurityManager());
	try {
	    String name = CLIENT_STUB_INTERFACE;
	    Registry registry = LocateRegistry.getRegistry(HOST);
	    Service serve = (Service) registry.lookup(name);
	    Client temp = new Client();
	    temp.startThread();
	    System.out.println("Client: sending request to server");
	    serve.execute(doodie);
	    System.out.println("Client: received response from server");
	} catch (Exception e) {
	    System.err.println("Client: sadface :(");
	    e.printStackTrace();
	}
    }

    private class PrintingThread implements Runnable
    {
	@Override
	public void run() {
	    while (true) {
		System.out.println(Client.doodie);
		try {
		    Thread.sleep(1000); // sleep for 1 second
		} catch (InterruptedException e) {
		    e.printStackTrace();
		}
	    }
	}
    }
}
