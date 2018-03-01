package hello.client;

import hello.server.Hello;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

public class HelloClient
{
    private static Hello stub;


    private static void makeCallWithTiemout() {
	ExecutorService executor = Executors.newSingleThreadExecutor();
	Future<String> future = executor.submit(new Callable<String>() {
	    public String call() throws Exception {
		String response = stub.sayHello();
		System.out.println("From inside the called method: " + response);
		return response;
	    }
	});

	try {
	    String response = future.get(2, TimeUnit.SECONDS);
	    System.out.println("response: " + response);
	} catch (InterruptedException | TimeoutException | ExecutionException e) {
	    System.err.println("RMI call timeout");
	    executor.shutdown();
	}
	System.out.println("Wait for 10 seconds: the response from the server will still come through!");
    }


    public static void main(String[] args) {
	if (args.length < 1) {
	    System.err.println("Usage: java HelloClient <host> [<registry-port>]");
	    System.exit(1);
	}

	String host = null;
	int registryPort = 1099;
	if (args.length == 1) {
	    host = args[0];
	} else {
	    host = args[0];
	    registryPort = Integer.parseInt(args[1]);
	}

	try {
	    Registry registry = LocateRegistry.getRegistry(host, registryPort);
	    stub = (Hello) registry.lookup("HelloServer");
	} catch (Exception e) {
	    System.err.println("Client exception: " + e.toString());
	    e.printStackTrace();
	}

	makeCallWithTiemout();
    }
}
