/**
 * 
 */
package time;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;
import java.util.Date;

/**
 * Server thread that listens for responses from the client
 * If it receives any multicast packets, it displays the
 * date on the console.
 * @author palderson
 *
 */
public class Server  extends Thread {

	public void run() {
		// join a Multicast group and send the group salutations

		InetAddress server;
		
		try {
			// Server receives answers on 5800 and sends on 5801
			server = InetAddress.getByName("230.230.230.230");
			MulticastSocket s = new MulticastSocket(5800);

			s.joinGroup(server);
			s.setSoTimeout(60000);
			

			// Loop and get the responses
			while (true) {

				byte[] buf = new byte[1500];
				DatagramPacket recv = new DatagramPacket(buf, buf.length);
				s.receive(recv); // Block until we get responses
				buf = recv.getData();
				Long clientDate = (Long) Helpers.getObject(buf);
				System.out.println("Client " + recv.getAddress().toString() + " thinks the time is " + clientDate.longValue());
			}
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (SocketTimeoutException e) {
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
