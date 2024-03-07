
/**
 * An example that shows that an inner class has separate monitor than
 * the outer class so synchronized keyword doesn't work across those classes!
 * 
 * @author amit
 */
public class SyncTest extends Thread {
	private int counter;
	
	public int getCounter() { return counter; }
	
	public synchronized void inc1() 
	{
		counter++;
	}
	
	public void run() {
		for (int i = 0; i < 100000; i++) {
			//System.out.println(counter);
			inc1();
		}
	}
	
	private class Inner extends Thread
	{
		public synchronized void inc2() {
			counter++;
		}
		
		public void run() 
		{
			for (int i = 0; i < 100000; i++) {
				inc2();
				System.out.println(counter);
			}
		}
	}

	public static void main(String[] args)
	{
		SyncTest tester1 = new SyncTest();
		tester1.start();
		
		SyncTest.Inner tester2 = tester1.new Inner();
		tester2.start();
		
		System.out.println(tester1.getCounter());
	}

}
