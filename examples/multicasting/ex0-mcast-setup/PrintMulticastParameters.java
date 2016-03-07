
import java.net.*;
import java.io.*;

public class PrintMulticastParameters
{
    public static void main(String args[]) throws IOException {
	if (args.length == 0) {
	    System.err.println("Usage: java PrintNetworkInterfaces <interface>");
	    System.exit(1);
	}
	InetAddress group = InetAddress.getByName("228.5.6.7");
	MulticastSocket mcast = new MulticastSocket(5500);
	mcast.joinGroup(group);

	System.out.println();
	System.out.println("interface used: " + mcast.getInterface());
	System.out.println("interface used: " + mcast.getNetworkInterface());
	System.out.println("Loopback support: " + mcast.getLoopbackMode());
	System.out.println();

	mcast.setLoopbackMode(true);
	NetworkInterface netif = NetworkInterface.getByName(args[0]);
	mcast.setNetworkInterface(netif);

	System.out.println();
	System.out.println("interface used: " + mcast.getInterface());
	System.out.println("interface used: " + mcast.getNetworkInterface());
	System.out.println("Loopback support: " + mcast.getLoopbackMode());
	System.out.println();
    }

}
