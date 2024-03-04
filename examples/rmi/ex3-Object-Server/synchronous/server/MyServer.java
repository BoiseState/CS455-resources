package synchronous.server;

import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.ServerNotActiveException;
import java.util.Date;

public class MyServer extends java.rmi.server.UnicastRemoteObject implements Server {

	private static final long serialVersionUID = -7592865213304639584L;

	public MyServer() throws RemoteException {
	}


	public Date getDate() throws RemoteException
	{
		printClientAddress();
		// helps to show that the default RMI implementation is multi-threaded
		System.out.println("MyServer: " + Thread.currentThread() + " going to sleep");
		try {
			Thread.sleep(2000); // 2 seconds
		} catch (InterruptedException e) {
			System.err.println(e);
		}
		System.out.println("MyServer:" + Thread.currentThread() + " waking up from sleep");
		return new Date();
	}


	public Object execute(WorkRequest work) throws RemoteException
	{
		return work.execute();
	}


	public StringEnumerationRequest getList() throws RemoteException
	{
		return new StringEnumerator(new String[] { "Goo", "Goo", "Gaa", "Gaa" });
	}


	private void printClientAddress()
	{
		try {
			System.out.println(getClientHost());
		} catch (ServerNotActiveException e) {
			System.err.println(e);
		}
	}


	public static void main(String args[])
	{

		if (args.length != 1) {
			System.err.println("Usage: java synchronous.server.MyServer <registry port>");
			System.exit(1);
		}
		int registryPort = Integer.parseInt(args[0]);
		// int poolSize = Runtime.getRuntime().availableProcessors() * 2;
		// System.setProperty("sun.rmi.transport.tcp.maxConnectionThreads", "" + "" + poolSize);
		try {
			Server server = new MyServer();

			Registry registry = LocateRegistry.getRegistry(registryPort);
			registry.rebind("NiftyObjectServer", server);
			System.out.println("NiftyObjectServer bound");

		} catch (RemoteException e) {
			System.err.println("MyServer: Problem registering server");
			System.err.println(e);
		}
	}
}
