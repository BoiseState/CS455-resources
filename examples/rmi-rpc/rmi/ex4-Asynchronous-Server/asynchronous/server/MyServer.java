package asynchronous.server;

import java.rmi.RMISecurityManager;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.ServerNotActiveException;
import java.util.Date;


public class MyServer extends java.rmi.server.UnicastRemoteObject implements Server 
{   
	private static final long serialVersionUID = 3781244179279385998L;

	public MyServer() throws RemoteException { }

    public Date getDate() throws RemoteException {
		printClientAddress();
		// helps to show that the default RMI implementation is multi-threaded
		System.out.println("MyServer: "+Thread.currentThread()+" going to sleep");
		try {Thread.sleep(1000);} catch (InterruptedException e) {}
		System.out.println("MyServer:"+Thread.currentThread()+" waking up from sleep");
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
        System.out.println("MyServer: started thread to execute request.");
	}

	private void printClientAddress()
	{
		try {System.out.println(getClientHost());}
		catch (ServerNotActiveException e) {System.out.println(e);}
	}

    public static void main(String args[]) 
	{
		int registryPort = 1099;
    	System.setProperty("java.security.policy", "mysecurity.policy");
		System.setSecurityManager(new RMISecurityManager());
		try {
			Server server = new MyServer();
			Registry registry = LocateRegistry.getRegistry(registryPort);
			registry.rebind("NiftyServer", server);
			System.err.println("MyServer: bound");
		} catch (java.io.IOException e) {
			System.err.println("MyServer: problem registering server");
			System.err.println(e);
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
            System.out.println("MyServer: async req");
            listener.workCompleted( request, result );
            System.out.println("MyServer: async complete");
        } catch (RemoteException e) {
            System.out.println("MyServer: AsyncExecuteThread:"+e);
        }
    }
}

