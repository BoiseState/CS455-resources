
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Date;

public class DateClient 
{
	private String serverName;

	public static void main(String[] argv) 
	{
		System.setProperty("javax.net.ssl.trustStore", "./resources/Client_Truststore");
		System.setProperty("java.security.policy", "./resources/mysecurity.policy");
		/*System.setSecurityManager(new RMISecurityManager());*/

		if (argv.length != 1) {
			System.out.println("usage: java DateClient <date server address>");
			System.exit(1);
		}

		DateClient client = new DateClient();
		client.setServerName(argv[0]);
		client.perform();
	}

	/**
	 *  
	 */
	private void perform() 
	{
		try {
			//bind server object to object in client
			Registry reg = LocateRegistry.getRegistry(2004);
			DateServer server = (DateServer) reg.lookup("DateServerImpl");
			System.out.println("RMI connection successful");

			//invoke method on server object
			Date result = server.getDate();
			System.out.println("The response from the server is "+result);
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("Exception occured: " + e);
			System.exit(0);
		}
	}

	public String getServerName() {
		return serverName;
	}

	public void setServerName(String serverName) {
		this.serverName = serverName;
	}
}
