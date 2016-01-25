import java.rmi.*;
import java.util.*;
import java.security.*;

public class MyServer 
	extends java.rmi.server.UnicastRemoteObject implements Server {   
 
    public MyServer() throws RemoteException { }


	// Implement the Server interface

    public Date getDate() throws RemoteException {
		return new Date();
	}
    public Object execute( WorkRequest work ) throws RemoteException {
		return work.execute();
	}
    public StringEnumeration getList() throws RemoteException {
		return new StringEnumerator( 
			new String [] { "Foo", "Bar", "Gee" } );
	}
    public void asyncExecute( WorkRequest request , WorkListener listener ) 
		throws java.rmi.RemoteException 
	{
		new AsyncExecuteThread(request, listener).start();
		System.out.println("Started thread to execute request.");
	}

    public static void main(String args[]) {
		try {
			System.setSecurityManager(new RMISecurityManager());
			Server server = new MyServer();
			Naming.rebind("NiftyServer", server);
			System.out.println("bound");
		} catch (java.io.IOException e) {
			System.out.println("// Problem registering server");
			System.out.println(e);
		}
    }

}


class AsyncExecuteThread extends Thread 
{
	WorkRequest request;
	WorkListener listener;

	public AsyncExecuteThread(WorkRequest request, WorkListener listener)
	{
		this.request = request;
		this.listener = listener;
	}

	public void run()
	{
		try {
			Object result = request.execute();
			System.out.println("async req");
			listener.workCompleted( request, result );
			System.out.println("async complete");
		} catch (RemoteException e) {
			System.out.println("AsyncExecuteThread:"+e);
		}
	}
}
