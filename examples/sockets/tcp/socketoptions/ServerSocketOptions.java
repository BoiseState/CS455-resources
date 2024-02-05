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

			System.out.println("ServerSocketOptions: ServerSocket bound");
			
			System.out.println("SO_TIMEOUT = " + s.getSoTimeout());
			s.setSoTimeout(30000); // timeout after 30 seconds
			System.out.println("ServerSocketOptions: New SO_TIMEOUT = " + s.getSoTimeout());
			
			System.out.println("ServerSocketOptions: Receive Buffer Size = " + s.getReceiveBufferSize());
			
			System.out.println("ServerSocketOptions: SO_REUSEADDR  = " + s.getReuseAddress());

			serveClients(s);

		} catch (IOException e) {
			System.err.println(e);
		}
	}


	private static void serveClients(ServerSocket s) {
		boolean shutdown = false;

		while (!shutdown) {
			try {
				Socket conn = s.accept();
				System.out.println("ServerSocketOptions: Connect from " + conn.getInetAddress());
			} catch (InterruptedIOException e) {
				System.err.println("ServerSocketOptions: accept timed out");
				shutdown = true;
			} catch (IOException e) {
				System.err.println(e);
			}
		}
	}

}
