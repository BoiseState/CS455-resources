import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;

/**
 * Modified from the original example from
 * http://www.dreamsyssoft.com/java-8-lambda-tutorial/.
 *
 */
public class UserTestWithLambdas {
	private static List<User> users = Arrays.asList(new User(1, "Alice", "Vai", 10), new User(4, "Anna", "Smith", 32),
	        new User(3, "Steve", "Johnson", 57), new User(9, "Mike", "Stevens", 18),
	        new User(10, "Alyssa", "Armstrong", 24), new User(2, "Jim", "Smith", 40),
	        new User(8, "Chuck", "Schneider", 34), new User(5, "Jorje", "Gonzales", 22),
	        new User(6, "Jane", "Michaels", 47), new User(7, "Kim", "Berlie", 60));

	public static void main(String[] args)
	{
		withLambdas();
	}


	private static void withLambdas()
	{
		Collections.sort(users, (User u1, User u2) -> u1.id.compareTo(u2.id));
		printListNewWay("by ID", users);

		Collections.sort(users, (User u1, User u2) -> u1.firstName.compareTo(u2.firstName));
		printListNewWay("by FirstName", users);

		Collections.sort(users, (User u1, User u2) -> u1.lastName.compareTo(u2.lastName));
		printListNewWay("by LastName", users);

		Collections.sort(users, (User u1, User u2) -> u1.age.compareTo(u2.age));
		printListNewWay("by Age", users);

		List<User> underageUsers = users.stream().filter(u -> u.age < 21).collect(Collectors.toList());
		printListNewWay("underage users", underageUsers);

		// stream is sequential, parallelStream is in parallel
		// List<Integer> binAges = users.stream().map(u ->
		// u.age/10).collect(Collectors.toList());
		List<Integer> binAges = users.parallelStream().map(u -> u.age / 10).collect(Collectors.toList());
		binAges.forEach(u -> System.out.println("bin: " + u));

		double average = users.parallelStream().map(u -> u.age).reduce(0, (a, b) -> a + b) / (double) users.size();
		System.out.println("Average age = " + average);
	}


	private static void printListNewWay(String type, List<User> users)
	{
		System.out.println("New Way " + type + ":");
		users.forEach(u -> System.out.println("\t" + u));
		System.out.println();
	}
}
