package sslsockets;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import javax.net.ssl.SSLSocket;
import javax.net.ssl.SSLSocketFactory;

public class Client
{
    public static void main(String argv[]) {
        if (argv.length != 2) {
            System.err.println("Usage: java Client <hostname> <port>");
            System.exit(1);
        }
        try {
            System.setProperty("javax.net.ssl.trustStore", "sslsockets/resources/Client_Truststore");
            System.setProperty("javax.net.ssl.trustStorePassword", "test123");

            int port = Integer.parseInt(argv[1]);
            SSLSocketFactory sslFact = (SSLSocketFactory) SSLSocketFactory.getDefault();
            SSLSocket server = (SSLSocket) sslFact.createSocket(argv[0], port);

            ObjectOutputStream out = new ObjectOutputStream(server.getOutputStream());
            ObjectInputStream in = new ObjectInputStream(server.getInputStream());

            out.writeObject(new DateRequest());
            out.flush();
            System.out.println(in.readObject());

            out.writeObject(new MyCalculation(5));
            out.flush();
            Object obj = in.readObject();
            if (obj instanceof int[]) {
                System.out.println(obj);

                int[] results = (int[]) obj;
                String padString = " ";
                for (int i = 0; i < results.length; i++) {
                    System.out.print(results[i] + padString);
                }
                System.out.println();
            }

            server.close();
        } catch (IOException e) {
            System.out.println("I/O error " + e); // I/O error
        } catch (ClassNotFoundException e2) {
            System.out.println(e2); // Unknown type of response object
        }
    }
}
