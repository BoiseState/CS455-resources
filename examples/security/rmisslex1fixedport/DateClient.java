package rmisslex1fixedport;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Date;

/**
 * Shows how to restrict all RMI calls to a single port for ease in dealing with firewalls. Note that the 
 * rmiregistry has to be on a different port. It is possible to have that be on the same port as well.
 * 
 * @author amit
 *
 */
public class DateClient
{
    /**
     * @param argv address for date server
     */
    public static void main(String[] argv) 
    {
	System.setProperty("javax.net.ssl.trustStore", "rmisslex1timeout/resources/Client_Truststore");
	System.setProperty("java.security.policy", "rmisslex1timeout/resources/mysecurity.policy");

	if (argv.length != 1) {
	    System.out.println("usage: java DateClient <date server address>");
	    System.exit(1);
	}

	DateClient client = new DateClient();
	client.perform();
    }

    /**
     *  Locate server and perform RMI call. Print server object to verify the port used is
     *  the one set in the Server's custom socket factory.
     */
    private void perform()
    {
	try {
	    // bind server object to object in client
	    Registry reg = LocateRegistry.getRegistry(DateServerImpl.REGISTRY_PORT);
	    DateServer server = (DateServer) reg.lookup("DateServerImpl");
	    
	    System.out.println("RMI connection successful:");
	    System.out.println(server); // so we can see the port assigned
	    
	    // invoke method on server object
	    Date result = server.getDate();
	    System.out.println("RMI response from the server is: " + result);
	} catch (Exception e) {
		System.err.println(e.getMessage());
		System.err.println("Exception occured: " + e);
	} 
    }
}
