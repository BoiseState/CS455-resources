package hello.server;


import java.rmi.*;
import java.rmi.registry.*;
import java.rmi.server.UnicastRemoteObject;

public class HelloServer extends UnicastRemoteObject implements Hello
{
   /**
	 * 
	 */
	private static final long serialVersionUID = 8510538827054962873L;
		private String name;
		private static int registryPort = 1099;

        public HelloServer(String s) throws RemoteException {
                super();
                name = s;
        }

        public String sayHello() throws RemoteException {
                return  "Hello World!";
        }

        public static void main(String args[])
        {
				if (args.length > 0) {
					registryPort = Integer.parseInt(args[0]);
				}

                try {
                		// Create and install a security manager
                		System.setSecurityManager(new RMISecurityManager());
						Registry registry = LocateRegistry.getRegistry(registryPort);
                        HelloServer obj = new HelloServer("//HelloServer");
                        registry.rebind("HelloServer", obj);
                        System.out.println("HelloServer bound in registry");
                } catch (Exception e) {
                        System.out.println("HelloServer err: " + e.getMessage());
                        e.printStackTrace();
                }
        }
}
