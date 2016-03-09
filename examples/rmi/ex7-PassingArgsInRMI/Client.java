import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * The entire point of this program is to show how parameters are passed in
 * RMI. 
 * @author atulepbe
 *
 */
public class Client {
	private final static String CLIENT_STUB_INTERFACE = "Service";
	private final static String HOST = "localhost";
	static Doodle doodie = new Doodle();

	public Client() {
		// for some reason Eclipse argued when I simply put this line in the main,
		// so moved it to constructor.
		new Thread(new PrintingThread(this)).start();
	}

	public static void main(String args[]) {
		if (System.getSecurityManager() == null)
			System.setSecurityManager(new SecurityManager());
		try {
			String name = CLIENT_STUB_INTERFACE;
			Registry registry = LocateRegistry.getRegistry(HOST);
			Service serve = (Service) registry.lookup(name);
			Client temp = new Client();
			System.out.println("Sending request to server");
			doodie = serve.execute(doodie);
			System.out.println("Received response from server");
		} catch (Exception e) {
			System.err.println("Sadface");
			e.printStackTrace();
		}
	}

	private class PrintingThread implements Runnable {
		Client doodleman;

		public PrintingThread(Client doodleman) {
			this.doodleman = doodleman; // this ensures that our doodie is always current.
		}

		@Override
		public void run() {
			while (true) {
				System.out.println(doodleman.doodie);
				try {
					Thread.sleep(5000); // sleep for 5 seconds
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}

		}

	}
}
