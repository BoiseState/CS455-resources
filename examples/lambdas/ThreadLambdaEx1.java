/**
 * Use of lambdas to replace anonymous classes for threads.
 * 
 * @author amit
 *
 */

public class ThreadLambdaEx1 {
	public static void main(String[] args)
	{
		Runnable r1 = new Runnable() {
			@Override
			public void run()
			{
				System.out.println("No lambda");
			}
		};

		Runnable r2 = () -> {
			System.out.println("With Lambda");
		};

		new Thread(r1).start();
		new Thread(r2).start();

		new Thread(() -> System.out.println("With Lambda as an argument!")).start();
	}
}