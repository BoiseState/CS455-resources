

import java.util.Random;

/**
 *  Example to demonstrate speed difference between creating physical threads and virtual threads. 
 *  Adapted from https://www.infoworld.com/article/3678148/intro-to-virtual-threads-a-new-approach-to-java-concurrency.html
 *  
 *  @author amit
 */
public class VirtualThreadsDemo
{
    public static void main(String[] args)
    {
	if (args.length != 1) {
	    System.err.println("Usage: java VirtualThreadsDemo <virtual|physical>");
	    System.exit(1);
	}
	boolean vThreads = true;
	if (args[0].toUpperCase().startsWith("P")) {
	    vThreads = false;
	    System.out.println("Using physical threads");
	} else {
	    vThreads = true;
	    System.out.println("Using virtual threads");
	}

	long start = System.currentTimeMillis();

	Random random = new Random();
	Runnable runnable = () -> {
	    //do some inconsequential computation
	    int sum = 0;
	    for (int i = 0; i < 10; i++) {
		sum += random.nextInt();
	    }
	    double avg = sum/1000;
	};
	
	for (int i = 0; i < 50000; i++) {
	    if (vThreads) {
		Thread.startVirtualThread(runnable);
	    } else {
		new Thread(runnable).start();
	    }
	}

	long finish = System.currentTimeMillis();
	long timeElapsed = finish - start;
	System.out.println("Run time: " + timeElapsed);
    }
}
