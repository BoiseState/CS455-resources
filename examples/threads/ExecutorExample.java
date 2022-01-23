import java.util.Arrays;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

/**
 * A simple example of using an ExecutorService to manage a thread pool.
 * 
 * @author amit
 */
public class ExecutorExample {
	public static final int NUM_THREADS = 10;
	public static final int EXIT_FAILURE = 1;
	public static final ExecutorService pool = Executors.newFixedThreadPool(NUM_THREADS);

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		if (args.length != 2) {
			System.err.println("Usage: java ExecutorExample <n> <#tasks>");
			System.exit(EXIT_FAILURE);
		}

		int n = Integer.parseInt(args[0]);
		int numTasks = Integer.parseInt(args[1]);
		MyCalculation[] tasks = new MyCalculation[numTasks];

		for (int i = 0; i < numTasks; i++) {
			tasks[i] = new MyCalculation(n);
			pool.execute(tasks[i]);
		}
		//shutdown the thread pool so it will not accept new tasks we don't really need it in this example 
		//but it would be good in general when we are ending a program or a service. See the docs for
		//ExecutorService for more details.
		pool.shutdown(); 		
		try {
            pool.awaitTermination(Long.MAX_VALUE, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            System.err.println("ExecutorExample: awaitTermination interrupted, task not finished!");
            e.printStackTrace();
            System.exit(1);
        }

		for (MyCalculation task: tasks) {
			System.out.println(Arrays.toString(task.getValues()));
		}
	}
}
