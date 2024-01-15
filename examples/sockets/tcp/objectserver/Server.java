package tcp.objectserver;

import java.io.EOFException;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;

public class Server
{
    public static void main(String argv[]) throws IOException {
        if (argv.length < 1) {
            System.err.println("Usage: java Server <port>");
            System.exit(1);
        }
        ServerSocket serverSocket = new ServerSocket(Integer.parseInt(argv[0]));
        System.out.println("Server: ready");
        while (true)
            new ServerConnection(serverSocket.accept()).start();
    }
}

class ServerConnection extends Thread
{
    private Socket client;

    ServerConnection(Socket client) throws SocketException {
        this.client = client;
        setPriority(NORM_PRIORITY - 1);
    }


    public void run() {
        try {
            ObjectOutputStream out = new ObjectOutputStream(client.getOutputStream());
            ObjectInputStream in = new ObjectInputStream(client.getInputStream());
            System.out.println("Server: Received connect from " + client.getInetAddress().getHostAddress());

            while (true) {
                out.writeObject(processRequest(in.readObject()));
                out.flush();
            }
        } catch (EOFException e) { // Normal EOF
            try {
                client.close();
            } catch (IOException e1) {
                System.err.println(e1);
            }
        } catch (IOException e) {
            System.err.println("I/O error " + e); // I/O error
        } catch (ClassNotFoundException e) {
            System.err.println(e); // Unknown type of request object
        }
    }


    private Object processRequest(Object request) {
        if (request instanceof DateRequest)
            return new java.util.Date();
        else if (request instanceof WorkRequest)
            return ((WorkRequest) request).execute();
        else
            return null;
    }
}
