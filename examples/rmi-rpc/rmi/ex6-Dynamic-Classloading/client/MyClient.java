import java.rmi.*;
import java.security.*;

public class MyClient 
	extends java.rmi.server.UnicastRemoteObject implements WorkListener {

    public static void main(String [] args) throws RemoteException 
	{
		int n = 50;
		if (args.length == 2)
			n = Integer.parseInt(args[1]);

		new MyClient(args[0], n);
	}

	public MyClient(String host, int n) throws RemoteException {
		try {
			System.setSecurityManager(new mySecurityManager());
			Server server = (Server)Naming.lookup("//"+host+"/NiftyServer");

			System.out.println( server.getDate() );
			System.out.println( server.execute( new MyCalculation(2) ) );
			StringEnumeration se = server.getList();
			while ( se.hasMoreItems() )
				System.out.println( se.nextItem() );
			server.asyncExecute( new MyCalculation(n), this );
			server.asyncExecute( new MyCalculation(n), this );

		} catch (java.io.IOException e) { 
			// I/O Error or bad URL
			System.out.println(e);
		} catch (NotBoundException e) { 
			// NiftyServer isn't registered
			System.out.println(e);
		}
    }

	public void workCompleted( WorkRequest request, Object obj ) 
	throws RemoteException 
	{
		System.out.println("Async work result = " + obj);
		System.out.flush();

		int [] results = (int []) obj;
		System.out.println("received array of length "+results.length);
		if (results.length < 100) {
			String padString = " ";
			for (int i=0; i<results.length; i++)
			{
				System.out.print(results[i]+padString);
			}
			System.out.println();
		}
	}
}

class mySecurityManager extends RMISecurityManager
{
		public void checkConnect(String host, int port, Object context) {}
		public void checkPermission(Permission perm) {}
}

