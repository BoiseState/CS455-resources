

import java.util.UUID;


public class UUIDTest
{
	public static void main (String[] args)
	{
		int count = 1;
		if (args.length == 1) {
			count = Integer.parseInt(args[0]);
		}


		UUID id = UUID.randomUUID();
		System.out.println(id);
		System.err.println("version = " + id.version() + " variant = " + id.variant());

		long time = System.currentTimeMillis();
		for (int i=0; i<count; i++) {
			id = UUID.randomUUID();
			/*System.out.println(id);*/
		}
		time = System.currentTimeMillis() - time;
		System.out.printf("Average time to generate a UUID = %8.4f us\n", time*1000/(double)count);
	}
}
