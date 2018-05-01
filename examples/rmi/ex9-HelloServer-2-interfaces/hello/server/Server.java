package hello.server;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class Server 
{
    private static final long serialVersionUID = 8510538827054962873L;
    private static int registryPort = 1099;

    public static void main(String args[]) {
	if (args.length > 0) {
	    registryPort = Integer.parseInt(args[0]);
	}

	try {
	    // Create and install a security manager
	    System.setSecurityManager(new SecurityManager());
	    Registry registry = LocateRegistry.getRegistry(registryPort);
	    
	    HelloServer obj1 = new HelloServer("//EnglishServer");
	    BonjourServer obj2 = new BonjourServer("//FrenchServer");
	    
	    registry.rebind("EnglishServer", obj1);
	    registry.rebind("FrenchServer", obj2);
	    
	    System.out.println("Server bound in registry");
	} catch (Exception e) {
	    System.out.println("Server err: " + e.getMessage());
	    e.printStackTrace();
	}
    }
}
