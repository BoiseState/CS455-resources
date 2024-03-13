import java.net.NetworkInterface;
import java.net.SocketException;

public class PrintNetworkInterfaces
{
    public static void main(String args[]) throws SocketException {
        if (args.length == 0) {
            System.err.println("Usage: java PrintNetworkInterfaces <interface>");
            System.exit(1);
        }
        NetworkInterface net1 = NetworkInterface.getByName(args[0]);
        System.out.println(net1);
        System.out.println("Support multicast=" + net1.supportsMulticast());

        NetworkInterface net3 = NetworkInterface.getByName("lo");
        System.out.println(net3);
        System.out.println("Support multicast=" + net3.supportsMulticast());
    }

}
