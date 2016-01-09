
import java.util.*;

public class MyCalculation extends WorkRequest 
{
	private int n;
	private int [] values;
	private final int MAX = 100;

	public MyCalculation( int n ) {
		this.n = n;
		values = new int[n];
	}
	public Object execute() 
	{
		for (int i=0; i<values.length; i++)
		{
			values[i] = (int) (Math.random() * MAX);
		}

		Arrays.sort(values);
		return values;
	}
}

