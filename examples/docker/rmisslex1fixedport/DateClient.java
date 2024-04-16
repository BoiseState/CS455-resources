
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Date;

/**
 * Shows how to restrict all RMI calls to a single port for ease in dealing with
 * firewalls. Note that the rmiregistry is on a different port. It is possible
 * to have that be on the same port as well.
 * 
 * @author amit
 *
 */
public class DateClient
{
    /**
     * @param argv
     *            address for date server
     */
    public static void main(String[] argv) {
        System.setProperty("javax.net.ssl.trustStore", "resources/Client_Truststore");
        System.setProperty("javax.net.ssl.trustStorePassword", "test123");
        System.setProperty("java.security.policy", "resources/mysecurity.policy");

        int registryPort = DateServerImpl.REGISTRY_PORT;
        if (argv.length != 2) {
            System.out.println("usage: java DateClient <date server address> [<rmiregistry port>]");
            System.exit(1);
        } 
        registryPort = Integer.parseInt(argv[1]);

        DateClient client = new DateClient();
        client.perform(registryPort);
    }


    /**
     * Locate server and perform RMI call. Print server object to verify the port
     * used is the one set in the Server's custom socket factory.
     */
    private void perform(int registryPort) {
        try {
            // bind server object to object in client
            Registry reg = LocateRegistry.getRegistry(registryPort);
            DateServer server = (DateServer) reg.lookup("DateServerImpl");

            System.out.println("\nDateClient: RMI connection successful.\n");
            System.out.println(server); // so we can see the port assigned
            System.out.println();

            // invoke method on server object
            Date result = server.getDate();
            System.out.println("DateClient: RMI response from the server is: " + result + "\n");
        } catch (Exception e) {
            System.err.println(e.getMessage());
            System.err.println("Exception occured: " + e);
        }
    }
}
