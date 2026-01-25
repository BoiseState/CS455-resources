package tcp.multithreaded;

import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.net.InetAddress;

/**
 * A multithreaded time server using an Executor pattern along with Lambdas to
 * simplify expressing the code.
 * 
 * @author amit
 */
public class TimeServerExecutorLambda {
	private ServerSocket serverSocket;
	public static ExecutorService pool;

	/**
	 * Creates a server socket that listens on the specified port number.
	 * 
	 * @param port The port number for the server.
	 */
	public TimeServerExecutorLambda(int port, int numThreads) {
		try {
			pool = Executors.newFixedThreadPool(numThreads);
			serverSocket = new ServerSocket(port);
			System.out.println("TimeServer: up and running on port " + port + " " + InetAddress.getLocalHost());
		} catch (IOException e) {
			System.err.println(e);
		}
	}

	/**
	 * The main server method that accepts connections and starts off a new thread
	 * to handle each accepted connection. Since this method never ends, we don't
	 * need to wait for the pool to finish its tasks,
	 */
	public void runServer() {
		try {
			while (true) {
				final Socket client = serverSocket.accept();
				System.out.println("TimeServer: Received connect from " + client.getInetAddress().getHostName() + " [ "
						+ client.getInetAddress().getHostAddress() + " ] ");
				pool.execute(() -> run(client)); // lambda used here!
			}
		} catch (IOException e) {
			System.err.println(e);
		}
	}

	public void run(Socket client) {
		try {
			Thread currentThread = Thread.currentThread();
			currentThread.setPriority(Thread.NORM_PRIORITY - 1);
			System.out.println("Running on thread " + currentThread.getName());

			OutputStream out = client.getOutputStream();
			ObjectOutputStream objout = new ObjectOutputStream(out);

			objout.writeObject(new java.util.Date());
			objout.flush();

			Thread.sleep(4000); // delay for demonstration purposes
			client.close();
		} catch (InterruptedException e) {
			System.out.println(e);
		} catch (IOException e) {
			System.out.println("I/O error " + e);
		}
	}

	public static void main(String args[]) {
		if (args.length < 2) {
			System.err.println("Usage: java TimeServer <port> <#threads>");
			System.exit(1);
		}
		int port = Integer.parseInt(args[0]);
		int numThreads = Integer.parseInt(args[1]);

		TimeServerExecutorLambda server = new TimeServerExecutorLambda(port, numThreads);
		server.runServer();
	}
}
