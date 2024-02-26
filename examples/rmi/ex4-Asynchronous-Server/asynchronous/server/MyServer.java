package asynchronous.server;

import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.ServerNotActiveException;
import java.util.Date;

/**
 * Shows a RMI server implementation. Also demonstrates the use a custom
 * security manager.
 *
 */
public class MyServer extends java.rmi.server.UnicastRemoteObject implements Server {
	private static final long serialVersionUID = 3781244179279385998L;

	/**
	 * A constructor is required when we extend UnicastRemoteObject
	 * 
	 * @throws RemoteException
	 */
	public MyServer() throws RemoteException {
	}


	public Date getDate() throws RemoteException
	{
		printClientAddress();
		// helps to show that the default RMI implementation is multi-threaded
		System.out.println("MyServer: " + Thread.currentThread() + " going to sleep");
		try {
			Thread.sleep(1000);
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
		return new StringEnumerator(new String[] { "Foo", "Bar", "Gee" });
	}


	public void asyncExecute(WorkRequest request, WorkListener listener) throws java.rmi.RemoteException
	{
		new AsyncExecuteThread(request, listener).start();
		System.out.println("MyServer: started thread to execute request.");
	}


	private void printClientAddress()
	{
		try {
			System.out.println(getClientHost());
		} catch (ServerNotActiveException e) {
			System.out.println(e);
		}
	}

	private class AsyncExecuteThread extends Thread {
		WorkRequest request;
		WorkListener listener;

		public AsyncExecuteThread(WorkRequest request, WorkListener listener) {
			this.request = request;
			this.listener = listener;
		}


		public void run()
		{
			try {
				Object result = request.execute();
				System.out.println("MyServer: async req");
				listener.workCompleted(request, result);
				System.out.println("MyServer: async complete");
			} catch (RemoteException e) {
				System.out.println("MyServer: AsyncExecuteThread:" + e);
			}
		}
	}

	public static void main(String args[])
	{
		int registryPort = 1099;
		if (args.length != 1) {
			System.err.println("Usage: java java MyServer <registry port>");
			System.exit(1);
		}
		registryPort = Integer.parseInt(args[0]);
		System.setProperty("java.security.policy", "mysecurity.policy");

		try {
			Server server = new MyServer();

			Registry registry = LocateRegistry.getRegistry(registryPort);
			registry.rebind("AsyncServer", server);
			System.err.println("MyServer: bound");

		} catch (java.io.IOException e) {
			System.err.println("MyServer: problem registering server");
			System.err.println(e);
		}
	}

}
