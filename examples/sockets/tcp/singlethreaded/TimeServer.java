package tcp.singlethreaded;

import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.InetAddress;

/**
 * A single-threaded time server that sends Date objects to clients. You may
 * need to open port 5005 in the firewall on the host machine (unless you are
 * locally).
 * 
 * @author amit
 */

public class TimeServer
{
    private InputStream in;
    private OutputStream out;
    private int port = 5005;
    private ServerSocket s;

    public static void main(String args[]) {
        TimeServer server = new TimeServer();
        server.serviceClients();
    }


    public TimeServer() {
        try {
            s = new ServerSocket(port);
            System.out.println("TimeServer: up and running on port " + port + " " + InetAddress.getLocalHost());
        } catch (IOException e) {
            System.err.println(e);
        }
    }


    /**
     * The method that handles the clients, one at a time.
     */
    public void serviceClients() {
        Socket sock;

        while (true) {
            try {
                sock = s.accept();
                out = sock.getOutputStream();
                in = sock.getInputStream();

                // Note that client gets a temporary/transient port on it's side
                // to talk to the server on its well known port
                System.out.println("TimeServer: Received connect from " + sock.getInetAddress().getHostAddress() + ": "
                        + sock.getPort());

                ObjectOutputStream oout = new ObjectOutputStream(out);
                oout.writeObject(new java.util.Date());
                oout.flush();

                Thread.sleep(4000); // 4 secs
                sock.close();
            } catch (InterruptedException e) {
                System.err.println(e);
            } catch (IOException e) {
                System.err.println(e);
            }
        }
    }
}
