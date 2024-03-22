/**
 * P2P application that uses multicasts to send timestamps between nodes. 
 */
package time;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.MulticastSocket;
import java.net.SocketAddress;
import java.net.UnknownHostException;

/**
 * Client thread that listens for requests from the server If it receives any
 * multicast packets, it multicasts the date to any listening servers.
 *
 */
public class Client extends Thread {

	@SuppressWarnings("resource")
	public void run()
	{
		SocketAddress client;
		final int MULTICAST_CLIENT_PORT = 5800;
		// Server receives answers on 5800 and sends on 5801
		try {
			InetAddress clientAddr = InetAddress.getByName("230.230.230.230");
			client = new InetSocketAddress(clientAddr, MULTICAST_CLIENT_PORT);
			InetAddress serverAddr = InetAddress.getByName("230.230.230.230");

			MulticastSocket s = new MulticastSocket(5800);
			MulticastSocket c = new MulticastSocket(5801);
			c.joinGroup(client, null);
			c.setSoTimeout(60000);

			byte[] buf = new byte[1000];
			DatagramPacket recv = new DatagramPacket(buf, buf.length);
			while (true) {
				c.receive(recv); // block until we get a request
				Long current = System.currentTimeMillis();
				byte bytes[] = Helpers.getBytes(current);
				DatagramPacket response = new DatagramPacket(bytes, bytes.length, 
															 serverAddr, 5800);
				s.send(response);
			}
		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
