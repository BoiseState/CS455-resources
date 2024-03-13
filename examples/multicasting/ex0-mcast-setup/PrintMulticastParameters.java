import java.io.IOException;
import java.net.MulticastSocket;
import java.net.NetworkInterface;
import java.net.SocketAddress;
import java.net.InetSocketAddress;
import java.net.InetAddress;

public class PrintMulticastParameters
{
    public static void main(String args[]) throws IOException {
        if (args.length == 0) {
            System.err.println("Usage: java PrintNetworkInterfaces <interface>");
            System.exit(1);
        }
        SocketAddress group = new InetSocketAddress(InetAddress.getByName("228.5.6.7"), 5500);
        MulticastSocket mcast = new MulticastSocket(5500);
        mcast.joinGroup(group, null);

        System.out.println();
        
        System.out.println("interface used: " + mcast.getNetworkInterface());
        System.out.println("Loopback support: " + mcast.getLoopbackMode());
        System.out.println();

        mcast.setLoopbackMode(true);
        NetworkInterface netif = NetworkInterface.getByName(args[0]);
        mcast.setNetworkInterface(netif);

        System.out.println();
        
        System.out.println("interface used: " + mcast.getNetworkInterface());
        System.out.println("Loopback support: " + mcast.getLoopbackMode());
        System.out.println();
        mcast.close();
    }

}
