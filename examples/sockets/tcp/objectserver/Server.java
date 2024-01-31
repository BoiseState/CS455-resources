package tcp.objectserver;

import java.io.EOFException;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;

/**
 * A general object-based server. Creates a thread to handle each client. This class should be improved to
 * limit the number of threads using a thread pool.
 * 
 * @author amit
 * 
 */
public class Server
{
    private long clientNum = 1;
    private int port = 0;
    
    public Server(int port) {
	this.port = port;
    }
    
    /**
     * The main method for the server -- runs forevere or until the client closes the connection.
     * @throws IOException
     */
    public void runServer() throws IOException
    {
	@SuppressWarnings("resource")
	ServerSocket serverSocket = new ServerSocket(port);
	System.out.println("Server: ready");
	while (true) {
	    new ServerConnection(serverSocket.accept(), clientNum).start();
	    System.out.println("Server: Started client#" + clientNum);
	    clientNum++;
	}
    }
    
    /**
     *A thread that handles one client.
     */
    private class ServerConnection extends Thread
    {
        private Socket client;
        private long clientNum;

        ServerConnection(Socket client, long requestNum) throws SocketException {
            this.clientNum = requestNum;
            this.client = client;
            setPriority(NORM_PRIORITY - 1);
        }


        public void run() {
            try {
                ObjectOutputStream out = new ObjectOutputStream(client.getOutputStream());
                ObjectInputStream in = new ObjectInputStream(client.getInputStream());
                System.out.println("ServerConnection: Received connect from " + client.getInetAddress().getHostAddress());

                while (true) {
                    out.writeObject(processRequest(in.readObject()));
                    out.flush();
                }
            } catch (EOFException e) { // Normal EOF
                try {
                    client.close();
                    System.out.println("ServerConnection: Finished client#" + clientNum);
                } catch (IOException e1) {
                    System.err.println(e1);
                }
            } catch (IOException e) {
                System.err.println("I/O error " + e); // I/O error
            } catch (ClassNotFoundException e) {
                System.err.println(e); // Unknown type of request object
            }
        }


        /**
         * Handles one request from a client.
         * @param request  A request object from the client.
         * @return
         */
        private Object processRequest(Object request) {
            if (request instanceof DateRequest)
                return new java.util.Date();
            else if (request instanceof WorkRequest)
                return ((WorkRequest) request).execute();
            else
                return null;
        }
    }

    
    public static void main(String argv[]) throws IOException {
        if (argv.length < 1) {
            System.err.println("Usage: java Server <port>");
            System.exit(1);
        }
        int port = Integer.parseInt(argv[0]);
        Server objserver = new Server(port);
        objserver.runServer();
       
    }
}

