package tcp.client;

import java.io.IOException;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

/**
 * A multithreaded port scanner using a thread pool.
 * 
 * @author amit
 *
 */
public class PortScannerThreadPoolLambda
{
    private String host;
    private int start;
    private int end;
    private ExecutorService pool;
    
    public PortScannerThreadPoolLambda(String host, int start, int end, int numThreads) {
        this.host = host;
        this.start = start;
        this.end = end;
        pool = Executors.newFixedThreadPool(numThreads);
    }
    
    public void runScan() throws InterruptedException {
        for (int next = start; next <= end; next++) {
        	final int port = next;
            pool.execute(() -> scanPort(host, port));
        }
        pool.shutdown();
        pool.awaitTermination(Long.MAX_VALUE, TimeUnit.SECONDS);
    }
    
    public void scanPort(String host, int port) {
    	try {
            Socket sock = new Socket(host, port);
            System.out.println("Connected to port " + port + " with local address " 
                                + sock.getLocalPort() + sock.getLocalAddress());
            sock.close();
        } catch (IOException e) {
            //System.err.println("portScanner: failed to connect to port " + port + " :" + e);
        }
    }

    /**
     * @param args
     * @throws InterruptedException 
     */
    public static void main(String[] args) throws InterruptedException {

        if (args.length != 4) {
            System.err.println("Usage: java PortScanner <host> <start-port> <end-port> <numThreads>");
			System.exit(1);
        }
        String host = args[0];
        int start = Integer.parseInt(args[1]);
        int end = Integer.parseInt(args[2]);
        int numThreads = Integer.parseInt(args[3]);
        
        PortScannerThreadPoolLambda whiteHat = new PortScannerThreadPoolLambda(host, start, end, numThreads);
        whiteHat.runScan();
    }

}
