package rmisslex1timeout;

import java.net.SocketTimeoutException;
import java.rmi.UnmarshalException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Date;

public class DateClient {
	private String serverName;

	public static void main(String[] argv)
	{
		System.setProperty("javax.net.ssl.trustStore", "rmisslex1timeout/resources/Client_Truststore");
		System.setProperty("javax.net.ssl.trustStorePassword", "test123");
		System.setProperty("java.security.policy", "rmisslex1timeout/resources/mysecurity.policy");

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
			// bind server object to object in client
			Registry reg = LocateRegistry.getRegistry(getServerName(), 2004);
			DateServer server = (DateServer) reg.lookup("DateServerImpl");

			System.out.println("RMI connection successful: initiating RMI call");
			// invoke method on server object
			Date result = server.getDate();
			System.out.println("RMI response from the server is: " + result);
		} catch (UnmarshalException e) {
			if (e.getCause() instanceof SocketTimeoutException) {
				System.err.println("RMI call timed out: " + e);
			}
		} catch (Exception e) {
			System.err.println(e.getMessage());
			System.err.println("Exception occured: " + e);
		}
	}


	public String getServerName()
	{
		return serverName;
	}


	public void setServerName(String serverName)
	{
		this.serverName = serverName;
	}
}
