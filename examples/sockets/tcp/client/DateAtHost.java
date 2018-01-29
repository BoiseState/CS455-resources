package tcp.client;

import java.io.DataInputStream;
import java.io.IOException;
import java.net.Socket;

/**
 * May have to enable time service as follows: Install xinetd service as
 * follows:
 *
 * sudo dnf install xinetd
 *
 * Set disable flag to "no" in /etc/xinetd.d/time-stream. Start xinetd service
 * to use new configuration:
 *
 * systemctl start xinetd systemctl status xinetd
 *
 * When done, disable service by setting the disable flag to "yes" and reloading
 * xinetd.
 *
 * systemctl reload xinetd
 *
 * To setup the service to start automatically at boot up time, use:
 *
 * systemctl enable xinetd
 * 
 * @author amit
 *
 */
public class DateAtHost extends java.util.Date
{
    private static int timePort = 37;
    private static final long offset = 2208988800L; // Seconds from century to
                                                    // Jan 1, 1970 00:00 GMT


    public DateAtHost(String host, int port) throws IOException {
	Socket server = new Socket(host, port);
	DataInputStream din = new DataInputStream(server.getInputStream());
	int time = din.readInt();
	server.close();

	setTime((((1L << 32) + time) - offset) * 1000);
    }


    public DateAtHost(String host) throws IOException {
	this(host, timePort);
    }


    public static void main(String[] args) throws Exception {
	if (args.length == 0) {
	    throw new IOException("Usage: java DateAtHost <hostname>");
	}
	System.out.println(new DateAtHost(args[0]));
    }
}
