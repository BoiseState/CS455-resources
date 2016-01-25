package callback.server;

import java.rmi.RMISecurityManager;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.ServerNotActiveException;
import java.util.Date;

public class MyServer extends java.rmi.server.UnicastRemoteObject implements Server
{
	private static final long serialVersionUID = -7592865213304639584L;

	public MyServer() throws RemoteException {}

	public Date getDate() throws RemoteException
	{
		printClientAddress();
		// helps to show that the default RMI implementation is multi-threaded
		System.out.println("MyServer: " + Thread.currentThread() + " going to sleep");
		try {
			Thread.sleep(1000); // 1 second
		} catch (InterruptedException e) {
			System.err.println(e);
		}
		System.out.println("MyServer:" + Thread.currentThread()	+ " waking up from sleep");
		return new Date();
	}

	public Object execute(WorkRequest work) throws RemoteException
	{
		return work.execute();
	}

	public StringEnumeration getList() throws RemoteException
	{
		return new StringEnumerator(new String[] { "Foo", "Bar", "Gee" });
	}

	public void asyncExecute(WorkRequest request, WorkListener listener)
			throws java.rmi.RemoteException
	{
		Object result = request.execute();
		System.out.println("async req");
		listener.workCompleted(request, result);
		System.out.println("async complete");
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
		int registryPort = 1099;
		System.setProperty("java.security.policy", "mysecurity.policy");
		try {
			System.setSecurityManager(new RMISecurityManager());
			Server server = new MyServer();
			Registry registry = LocateRegistry.getRegistry(registryPort);
			registry.rebind("NiftyServer", server);
			System.out.println("bound");
		} catch (java.io.IOException e) {
			System.err.println("MyServer: Problem registering server");
			System.err.println(e);
		}
		
	}

}
