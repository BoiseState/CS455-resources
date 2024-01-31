import java.util.ArrayList;

public class SimpleLambdas {
	public static void main(String args[])
	{
		// Creating an ArrayList with elements
		// {1, 2, 3, 4}
		ArrayList<Integer> l = new ArrayList<Integer>();
		l.add(1);
		l.add(2);
		l.add(3);
		l.add(4);

		// Using lambda expression to print all elements of l
		l.forEach(n -> System.out.println(n));
		System.out.println();

		// Using lambda expression to print even elements of l
		l.forEach(n -> {
			if (n % 2 == 0) System.out.println(n);
		});
	}
}
