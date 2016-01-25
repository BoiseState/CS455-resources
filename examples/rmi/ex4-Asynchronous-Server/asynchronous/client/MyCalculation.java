package asynchronous.client;


import asynchronous.server.WorkRequest;

public class MyCalculation extends WorkRequest 
{
	private static final long serialVersionUID = -1211306562907927504L;
	private int [] values;
	
	public MyCalculation( int n ) {
		values = new int[n];
	}
	public Object execute() 
	{
		for (int i=0; i<values.length; i++)
		{
			values[i] = i*i;
		}
		return values;
	}
}

