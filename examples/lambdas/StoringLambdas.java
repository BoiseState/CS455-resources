import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

public class StoringLambdas {
	@SuppressWarnings("resource")
	public static void main(String[] args)
	{
		Map<Character, Runnable> commands = new HashMap<>();
		// Populate commands map
		commands.put('h', () -> System.out.println("Type h or q"));
		commands.put('q', () -> System.exit(0));
		while (true) {
			// Print menu
			System.out.println("Menu");
			System.out.println("h) Help");
			System.out.println("q) Quit");
			// User input
			char key = new Scanner(System.in).nextLine().charAt(0);
			// Run selected command
			if (commands.containsKey(key)) commands.get(key).run();
		}
	}
}
