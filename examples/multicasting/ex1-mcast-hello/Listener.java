
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.NetworkInterface;
import java.net.SocketTimeoutException;

/**
 * Multicast hello world listener
 */
public class Listener
{

    /**
     * @param args
     * @throws IOException
     **/
    public static void main(String[] args) throws IOException 
    {
	int n = 20;
	String networkInterface;
	if (args.length != 2) {
	    System.err.println("Usage: java Listener <n=packets> <network-interface>");
	    System.exit(1);
	}
	n = Integer.parseInt(args[0]);
	networkInterface = args[1];

	InetAddress group = InetAddress.getByName("230.230.230.230");
	MulticastSocket s = new MulticastSocket(6789);
	s.setSoTimeout(10000); // 10 seconds
	NetworkInterface net = NetworkInterface.getByName(networkInterface);
	s.setNetworkInterface(net);
	s.joinGroup(group);

	try {
	    int count = 0;
	    while (count < n) {
		byte[] buf = new byte[1024];
		DatagramPacket recv = new DatagramPacket(buf, buf.length);
		s.receive(recv);
		count++;
		System.out.println(
		        "Listener-- " + new String(buf) + "     packet# " + count + " from " + recv.getAddress());
		try {
		    Thread.sleep(1000);
		} catch (InterruptedException e) {
		    System.err.println(e);
		}
	    }
	} catch (SocketTimeoutException e) {
	    System.err.println(e);
	    s.leaveGroup(group);
	}
    }
}
