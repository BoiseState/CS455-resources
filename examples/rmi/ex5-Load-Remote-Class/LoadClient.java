
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
            URL url = new URI(args[0]).toURL();
            Class<?> cl = RMIClassLoader.loadClass(url, "client.RunAway");
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

