package largerhttpd;

import java.nio.channels.SelectionKey;
import java.util.ArrayList;

public class ClientQueue extends ArrayList<SelectionKey>
{
	public synchronized boolean add(SelectionKey key)
	{
		super.add(key);
		notify();
		return true;
	}

	public synchronized SelectionKey next()
	{
		while (isEmpty())
			try {
				wait();
			} catch (InterruptedException e) {
				System.err.println(e);
			}
		return remove(0);
	}
}
