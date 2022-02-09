package tcp.multithreaded;

import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInput;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;
import java.net.InetAddress;

/**
 * A multithreaded time servent (server+client) example. Run two copies in two terminals 
 * as follows:
 * 
 * java TimeServent 1 5005 localhost 5006
 * 
 * --> other terminal
 * 
 * java TimeServent 2 5006 localhost 5005
 * 
 * Make sure to to start the second servent within 10 seconds!
 * 
 * @author amit
 */
public class TimeServent
{
    private String name;
    private Server server;
    private Client client;

    /**
     * Creates a server socket that listens on the specified port number.
     * 
     * @param port
     *            The port number for the server.
     */
    public TimeServent(String myName, int myPort, String otherHost, int otherPort) {
        name = myName;
        server = new Server(myPort);
        client = new Client(otherHost, otherPort);
    }


    public void runServent() {
        server.start(); // server goes off to its own thread
        client.startTimer();
    }

    /**
     * The main server thread.  
     */
    private class Server extends Thread
    {
        private ServerSocket ss;

        /**
         * Creates a server socket that listens on the specified port number.
         * 
         * @param port
         *            The port number for the server.
         */
        public Server(int port) {
            try {
                ss = new ServerSocket(port);
                System.out.println(
                        "Servent" + name + ": up and running on port " + port + " " + InetAddress.getLocalHost());
            } catch (IOException e) {
                System.err.println(e);
            }
        }


        /**
         * The main server method that accepts connections and starts off a new thread
         * to handle each accepted connection.
         */
        public void run() {
            Socket client;
            System.out.println("Servent" + name + ": main thread" + getName());
            try {
                while (true) {
                    client = ss.accept();
                    System.out.println(
                            "Servent" + name + ": Received connect from " + client.getInetAddress().getHostName()
                                    + " [ " + client.getInetAddress().getHostAddress() + "," + client.getPort() + "] ");
                    new ServerConnection(client).start();
                }
            } catch (IOException e) {
                System.err.println(e);
            }
        }
    }

    /**
     * A thread for handling one connection.
     */
    private class ServerConnection extends Thread
    {
        private Socket client;

        ServerConnection(Socket client) throws SocketException {
            this.client = client;
            setPriority(NORM_PRIORITY - 1);
            System.out.println("Servent" + name + ": created thread " + this.getName());
        }


        /**
         *
         */
        public void run() {
            try {
                OutputStream out = client.getOutputStream();
                ObjectOutputStream oout = new ObjectOutputStream(out);

                oout.writeObject(new java.util.Date());
                oout.flush();

                // Thread.sleep(4000); // delay
                client.close();
            } catch (IOException e) {
                System.out.println("I/O error " + e);
            }
        }
    }

    /**
     * The main client class. Keeps connecting with the other servent and asks for time.
     */
    private class Client
    {
        private String host;
        private int port;
        private Timer timer = new Timer("client timer");
        private int delay = 4000;

        /**
         * @param host
         * @param port
         */
        public Client(String host, int port) {
            this.host = host;
            this.port = port;
                  
        }
        
        
        /**
         * setup timer task for a client
         */
        public void startTimer() {
            TimerTask task = new TimerTask() {
                public void run() {
                    runClient();
                }
            };
            timer.schedule(task, 0, delay); 
        }
        
        
        /**
         * 
         */
        public void runClient() {
            try {
                    Socket s = new Socket(host, port);
                    InputStream in = s.getInputStream();
                    ObjectInput oin = new ObjectInputStream(in);

                    Date date = (Date) oin.readObject();
                    System.out.println("Servent" + name + ": Time on other host " + host + " is " + date);
                    s.close();            
            } catch (IOException e1) {
                System.out.println("Client failed. Will try again.");
            } catch (ClassNotFoundException e2) {
                System.out.println(e2);
            }
        }
    }

    /**
     * The main where the server and client gets started.
     * 
     * @param args
     */
    public static void main(String args[]) {
        if (args.length < 4) {
            System.err.println("Usage: java TimeServer <myname> <myport> <server-hostname> <server-port>");
            System.exit(1);
        }
        String myName = args[0];
        int myPort = Integer.parseInt(args[1]);
        String otherHost = args[2];
        int otherPort = Integer.parseInt(args[3]);

        TimeServent servent = new TimeServent(myName, myPort, otherHost, otherPort);
        servent.runServent(); // runs forever
    }
}
