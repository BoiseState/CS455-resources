package tcp.socketoptions;

import java.io.IOException;
import java.io.InterruptedIOException;
import java.net.ServerSocket;
import java.net.Socket;

public class ServerSocketOptions
{
	private static final int BACKLOG = 1024;

	public static void main(String args[]) {
		try {
			ServerSocket s = new ServerSocket(5005, BACKLOG);

			System.out.println("ServerSocket");
			System.out.println("SO_TIMEOUT = " + s.getSoTimeout());
			System.out.println("Receive Buffer Size = " + s.getReceiveBufferSize());
			s.setSoTimeout(30000); // timeout after 30 seconds
			System.out.println("New SO_TIMEOUT = " + s.getSoTimeout());
			System.out.println("SO_REUSEADDR  = " + s.getReuseAddress());

			serverClients(s);

		} catch (IOException e) {
			System.err.println(e);
		}
	}


	private static void serverClients(ServerSocket s) {
		boolean shutdown = false;

		while (!shutdown) {
			try {
				Socket conn = s.accept();
				System.out.println("Connect from " + conn.getInetAddress());
			} catch (InterruptedIOException e) {
				System.err.println("accept timed out");
				shutdown = true;
			} catch (IOException e) {
				System.err.println(e);
			}
		}
	}

}
