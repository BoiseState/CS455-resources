package tcp.multithreaded;

import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.net.InetAddress;

/**
 * A multithreaded time server. This example shows how to limit the number of threads
 * used. It also shows how to make the thread class be an inner class. This is a nice
 * example of using synchronization as well.
 * 
 * @author amit
 */
public class TimeServerImproved
{
    private ServerSocket ss;
    private final int MAX_THREADS = 4;
    private int numThreads = 0;
    private Object lock = new Object();

    /**
     * Creates a server socket that listens on the specified port number.
     * 
     * @param port
     *            The port number for the server.
     */
    public TimeServerImproved(int port) {
        try {
            ss = new ServerSocket(port);
            System.out.println("TimeServer: up and running on port " + port + " " + InetAddress.getLocalHost());
        } catch (IOException e) {
            System.err.println(e);
        }
    }


    /**
     * Block if the number of threads equals the threshold.
     */
    public void checkMaxThreads() {
        while (numThreads >= MAX_THREADS) {
            synchronized (lock) {
                try {
                    lock.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }


    /**
     * The main server method that accepts connections and starts off a new thread
     * to handle each accepted connection.
     */
    public void runServer() {
        Socket client;
        try {
            while (true) {
                checkMaxThreads(); // block if max# threads are active
                client = ss.accept();
                System.out.println("TimeServer: Received connect from " + client.getInetAddress().getHostName() + " [ "
                        + client.getInetAddress().getHostAddress() + " ] ");
                incNumThreads();
                new ServerConnection(client).start();
            }
        } catch (IOException e) {
            System.err.println(e);
        }
    }


    private synchronized void incNumThreads() {
        numThreads++;
    }


    private synchronized void decNumThreads() {
        numThreads--;
    }

    /**
     * Handles one connection in a separate thread.
     */
    class ServerConnection extends Thread
    {
        private Socket client;

        ServerConnection(Socket client) throws SocketException {
            this.client = client;
            setPriority(NORM_PRIORITY - 1);
            System.out.println("Created thread " + this.getName());
        }


        public void run() {
            try {
                OutputStream out = client.getOutputStream();
                ObjectOutputStream oout = new ObjectOutputStream(out);

                oout.writeObject(new java.util.Date());
                oout.flush();

                Thread.sleep(4000); // delay
                client.close();
                decNumThreads();
                synchronized (lock) {
                    lock.notify();
                }

            } catch (InterruptedException e) {
                System.out.println(e);
            } catch (IOException e) {
                System.out.println("I/O error " + e);
            }
        }
    }

    public static void main(String args[]) {
        if (args.length < 1) {
            System.err.println("Usage: java TimeServer <port>");
            System.exit(1);
        }
        TimeServerImproved server = new TimeServerImproved(Integer.parseInt(args[0]));
        server.runServer();
    }
}
