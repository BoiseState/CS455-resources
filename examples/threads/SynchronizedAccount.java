
/**
 * Extend the base Account class and override the methods to
 * make them synchronized.
 * 
 */
public class SynchronizedAccount extends Account
{
	public synchronized void deposit(double amount)
	{
		super.deposit(amount);
		// balance += amount;
	}


	public synchronized void withdraw(double amount)
	{
		super.withdraw(amount);
		// balance -= amount;
	}
}
