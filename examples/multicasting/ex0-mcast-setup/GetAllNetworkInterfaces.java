import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Collections;
import java.util.Enumeration;

public class GetAllNetworkInterfaces
{
    public static void main(String[] args) {

        try {
            Enumeration<NetworkInterface> list = NetworkInterface.getNetworkInterfaces();
            while (list.hasMoreElements()) {
                NetworkInterface next = list.nextElement();
                System.out.println("Display Name: " + next.getDisplayName());
                System.out.println("Name: " + next.getName());
                Enumeration<InetAddress> inetAddresses = next.getInetAddresses();
                for (InetAddress inetAddress : Collections.list(inetAddresses)) {
                    System.out.printf("InetAddress: %s\n", inetAddress);
                }
                System.out.printf("\n");
            }
        } catch (SocketException e) {
            e.printStackTrace();
        }

    }

}
