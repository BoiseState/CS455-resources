
import java.rmi.server.RMIClassLoader;
import java.net.*;
import java.security.*;

public class LoadClient
{
    public static void main(String[] args) {
	if (args.length == 0) {
	    System.err.println("Usage: java LoadClient <remote URL>");
	    System.exit(1);
	}
	System.setSecurityManager(new mySecurityManager());
	try {
	    URL url = new URL(args[0]);
	    Class cl = RMIClassLoader.loadClass(url, "client.RunAway");
	    System.out.println(cl);
	    Runnable client = (Runnable) cl.newInstance();
	    client.run();
	    System.exit(0);
	} catch (Exception e) {
	    System.out.println("Exception: " + e.getMessage());
	    e.printStackTrace();
	}
    }
}

class mySecurityManager extends SecurityManager
{
    public void checkConnect(String host, int port, Object context) {
    }


    public void checkPermission(Permission perm) {
    }
}
