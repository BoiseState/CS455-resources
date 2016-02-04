package tinyhttpd;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * 
 * A simple web server that accepts basic HTTP commands. For example, try:
 * 
 * 	java tinyhttpd.TinyHttpd 5005
 * 
 * Or with security manager enabled:
 * 
 * 	java -Djava.security.manager -Djava.security.policy=mysecurity.policy TinyHttpd 5005 
 * 
 * Then point a web browser to localhost:5005
 * 
 * 	firefox localhost:5005
 * 
 * Or specify a path, such as
 * 
 * 	firefox localhost:5005/sample.html
 * 
 * Also try running the provided client as:
 * 
 * 	java tinyhttpd.Client localhost 5005
 * 
 * Or try a path in the server's root folder
 * 
 * 	java tinyhttpd.Client localhost sample.html
 *
 */
public class TinyHttpd
{
    private static final int EXIT_FAILURE = 1;

    public static void main(String argv[]) throws IOException
    {
	if (argv.length == 0) {
	    System.out.println("Usage: java TinyHttpd <port#>");
	    System.exit(EXIT_FAILURE);
	}

	ServerSocket ss = new ServerSocket(Integer.parseInt(argv[0]));
	while (true)
	    new Thread(new TinyHttpdConnection(ss.accept())).start();
    }
}

class TinyHttpdConnection implements Runnable
{
    private Socket client;
    private String basename = "tinyhttpd";

    TinyHttpdConnection(Socket client) throws SocketException
    {
	this.client = client;
    }

    public void run()
    {
	try {
	    BufferedReader in = new BufferedReader(new InputStreamReader(client.getInputStream(), "8859_1"));
	    OutputStream out = client.getOutputStream();
	    PrintWriter pout = new PrintWriter(new OutputStreamWriter(out, "8859_1"), true);
	    String request = in.readLine();
	    System.out.println("Request: " + request);

	    Matcher get = Pattern.compile("GET /?(\\S*).*").matcher(request);
	    if (get.matches()) {
		request = get.group(1);
		if (request.endsWith("/") || request.equals("")) request = request + "index.html";
		try {
		    request = basename + File.separator + request;
		    System.out.println(request);
		    FileInputStream fis = new FileInputStream(request);
		    byte[] data = new byte[64 * 1024];
		    for (int read; (read = fis.read(data)) > -1;)
			out.write(data, 0, read);
		    out.flush();
		} catch (FileNotFoundException e) {
		    pout.println("404 Object Not Found");
		} catch (SecurityException e) {
		    pout.println("403 Forbidden");
		}
	    } else
		pout.println("400 Bad Request");
	    client.close();
	} catch (IOException e) {
	    System.out.println("I/O error " + e);
	}
    }
}
