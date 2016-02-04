package tinyhttpd;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.Socket;

/**
 * A simple web client that reads the raw HTML from any web server.
 * For example, try:
 * 	java tinyhttpd.Client  cs.boisestate.edu 80
 * Then run the server in the same example. Suppose the server is using port 5005.
 * Then run the client as:
 * 	java tinyhttpd.Client localhost 5005
 * Or try a path in the server's root folder
 * 	java tinyhttpd.Client localhost sample.html
 */
public class Client
{
    public static void main(String[] args) throws IOException
    {
	if (args.length < 2) {
	    throw new IOException("\n Usage: java client <hostname> <port#> [pageurl]");
	}
	String hostName = args[0];
	int port = Integer.parseInt(args[1]);
	System.out.println("connecting...");
	Socket s = new Socket(hostName, port);
	String page;
	if (args.length < 3)
	    page = "index.html";
	else
	    page = args[2];
	new PrintStream(s.getOutputStream()).println("GET /" + page);
	/* DataInputStream bin = new DataInputStream( s.getInputStream() ); */
	BufferedReader bin = new BufferedReader(new InputStreamReader(s.getInputStream()));
	String line;
	while ((line = bin.readLine()) != null)
	    System.out.println(line);
	s.close();
    }
}