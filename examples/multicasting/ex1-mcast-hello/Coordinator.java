
import java.io.IOException;
import java.io.StringWriter;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.MulticastSocket;
import java.net.NetworkInterface;
import java.net.SocketAddress;

/**
 * Multicast hello world example.
 * @author amit
 *
 */
public class Coordinator
{

    public static final int MULTICAST_PORT = 6789;

    /**
     * @param args
     * @throws IOException
     **/
    public static void main(String[] args) throws IOException {
        int n = 20;
        String networkInterface;
        if (args.length != 2) {
            System.err.println("Usage: java Coordinator <n=packets> <network-interface>");
            System.exit(1);
        }
        n = Integer.parseInt(args[0]);
        networkInterface = args[1];

        String msg = "Hello";
        InetAddress addr = InetAddress.getByName("230.230.230.230");
        SocketAddress group = new InetSocketAddress(addr, MULTICAST_PORT);
        MulticastSocket s = new MulticastSocket(MULTICAST_PORT);
        NetworkInterface net = NetworkInterface.getByName(networkInterface);
        
        s.setNetworkInterface(net);
        s.joinGroup(group, null);

        int count = 0;
        while (count < n) {
            StringWriter str = new StringWriter();
            str.write(msg + ":" + count);
            DatagramPacket hi = new DatagramPacket(str.toString().getBytes(), 
            										str.toString().length(), 
            										addr, 
            										MULTICAST_PORT);
            s.send(hi);

            // get their responses!
            byte[] buf = new byte[1024];
            DatagramPacket recv = new DatagramPacket(buf, buf.length);
            s.receive(recv);
            count++;
            System.out.println(
                    "Coordinator-- " + new String(buf) + "     packet# " + count + " from " + recv.getAddress());
            try {
                Thread.sleep(1000);// 1 second
            } catch (InterruptedException e) {
                System.err.println(e);
            }
        }
        // OK, I'm done talking, leave the group...
        s.leaveGroup(group, net);
        s.close();
    }
}
