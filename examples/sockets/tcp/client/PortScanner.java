package tcp.client;

import java.io.IOException;
import java.net.Socket;

/**
 * A single threaded port scanner.
 * 
 * @author amit
 */
public class PortScanner
{
    private String host;
    private int start = 1;
    private int end = 65535;

    public PortScanner(String host, int start, int end)
    {
	this.host = host;
	this.start = start;
	this.end = end;
    }


    public void runScan()
    {
	for (int port = start; port <= end; port++) {
	   scanPort(host, port);
	}
    }


    private void scanPort(String host, int port)
    {
	try {
		Socket sock = new Socket(host, port);
		System.out.println("Connected to port " + port + " with local address " + sock.getLocalPort()
		        + sock.getLocalAddress());
		sock.close();
	    } catch (IOException e) {
		// System.err.println("portScanner: failed to connect to port " + port + " :" +
		// e);

	    }
    }


    /**
     * @param args
     */
    public static void main(String[] args)
    {

	if (args.length != 3) {
	    System.err.println("Usage: java PortScanner <host> <start-port> <end-port>");
	    System.exit(1);
	}
	String host = args[0];
	int start = Integer.parseInt(args[1]);
	int end = Integer.parseInt(args[2]);

	PortScanner whiteHat = new PortScanner(host, start, end);
	whiteHat.runScan();
    }

}
