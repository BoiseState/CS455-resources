/**
 * 
 */
package time;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.UnknownHostException;

/**
 * Main Application for this package. It spawns both a client and a server
 * thread. If the user presses enter, it sends out a request to the client to
 * announce themselves to the server.
 *
 */
public class Controller
{
    /**
     * Sends out the request when the user presses enter. Exits when they type "quit".
     * 
     * @param args none
     */
    public static void main(String[] args) 
    {
	String msg = "Send me your time.";
	String linein = "";
	new Client().start();
	new Server().start();

	InetAddress client = null;
	try {
	    client = InetAddress.getByName("230.230.230.230");
	} catch (UnknownHostException e) {
	    // TODO Auto-generated catch block
	    e.printStackTrace();
	}

	InputStreamReader converter = new InputStreamReader(System.in);
	BufferedReader in = new BufferedReader(converter);

	System.out.println("Press enter to request time (type 'quit' to exit): ");
	try {
	    MulticastSocket r = new MulticastSocket(5801);
	    r.joinGroup(client);
	    DatagramPacket request = new DatagramPacket(msg.getBytes(), msg.length(), client, 5801);
	    while (!(linein.equals("quit"))) {
		linein = in.readLine();
		if (!(linein.equals("quit"))) {
		    r.send(request);
		}
	    }
	} catch (IOException e) {
	    e.printStackTrace();
	}
    }
}
