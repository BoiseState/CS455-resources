
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
        try {
            String codebase = args[0];
            Class<?> cl = RMIClassLoader.loadClass(codebase, "client.RunAway");
            System.out.println(cl);
            Runnable client = (Runnable) cl.getDeclaredConstructor().newInstance();
            client.run();
            System.exit(0);
        } catch (Exception e) {
            System.out.println("Exception: " + e.getMessage());
            e.printStackTrace();
        }
    }
}


