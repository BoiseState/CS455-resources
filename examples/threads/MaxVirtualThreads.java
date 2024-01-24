
/**
 * Create a large number of virtual threads. Virtual threads are daemon threads
 * by default so we need to join on them to make sure they finish.
 * 
 * @author amit
 */
public class MaxVirtualThreads
{
	final static int MAX = 500000;
	private static Thread[] tids = new Thread[MAX];

	public static void main(String args[]) throws InterruptedException
	{
		for (int i = 0; i < MAX; i++) {
			Integer I = i;
			Runnable runnable = () -> {
				System.out.println("Thread number " + I);
				try {
					Thread.sleep(20000); // in millisecs
				} catch (InterruptedException e) {
					System.err.println(e);
				}
			};
			tids[i] = Thread.ofVirtual().start(runnable);
		}

		for (int i = 0; i < MAX; i++) {
			tids[i].join();
		}
	}
}
