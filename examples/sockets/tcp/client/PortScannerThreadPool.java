import java.io.IOException;
import java.net.Socket;
import java.util.concurrent.Executor;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

/**
 * A multithreaded port scanner using a thread pool.
 * 
 * @author amit
 *
 */
public class PortScannerThreadPool
{
    private String host;
    private int start = 1;
    private int end = 65535;
    private ExecutorService pool;
    
    public PortScannerThreadPool(String host, int start, int end, int numThreads) {
        this.host = host;
        this.start = start;
        this.end = end;
        pool = Executors.newFixedThreadPool(numThreads);
    }
    
    private class ScanPort implements Runnable {
        
        private String host;
        private int port;
        
        public ScanPort(String host, int port) {
            this.host = host;
            this.port = port;
        }

        @Override
        public void run() {
            try {
                Socket sock = new Socket(host, port);
                System.out.println("Connected to port " + port + " with local address " 
                                    + sock.getLocalPort() + sock.getLocalAddress());
                sock.close();
            } catch (IOException e) {
                //System.err.println("portScanner: failed to connect to port " + port + " :" + e);
                
            }         
        }
        
    }
    
    public void runScan() throws InterruptedException {
        for (int port = start; port <= end; port++) {
            pool.execute(new ScanPort(host, port));
        }
        pool.shutdown();
        pool.awaitTermination(Long.MAX_VALUE, TimeUnit.SECONDS);
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
        
        PortScannerThreadPool whiteHat = new PortScannerThreadPool(host, start, end, numThreads);
        whiteHat.runScan();
    }

}
