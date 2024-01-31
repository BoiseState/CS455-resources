import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.stream.Collectors;

/**
 * Modified from the original example from
 * http://www.dreamsyssoft.com/java-8-lambda-tutorial/.
 *
 */
public class UserTest {
	private static List<User> users = Arrays.asList(new User(1, "Alice", "Vai", 10), new User(4, "Anna", "Smith", 32),
	        new User(3, "Steve", "Johnson", 57), new User(9, "Mike", "Stevens", 18),
	        new User(10, "Alyssa", "Armstrong", 24), new User(2, "Jim", "Smith", 40),
	        new User(8, "Chuck", "Schneider", 34), new User(5, "Jorje", "Gonzales", 22),
	        new User(6, "Jane", "Michaels", 47), new User(7, "Kim", "Berlie", 60));

	public static void main(String[] args)
	{
		withoutLambdas();
	}


	private static void withoutLambdas()
	{
		Collections.sort(users, new Comparator<User>() {
			public int compare(User u1, User u2)
			{
				return u1.id.compareTo(u2.id);
			}
		});

		printList("by ID", users);

		Collections.sort(users, new Comparator<User>() {
			public int compare(User u1, User u2)
			{
				return u1.firstName.compareTo(u2.firstName);
			}
		});

		printList("by FirstName", users);

		Collections.sort(users, new Comparator<User>() {
			public int compare(User u1, User u2)
			{
				return u1.lastName.compareTo(u2.lastName);
			}
		});

		printList("by LastName", users);

		Collections.sort(users, new Comparator<User>() {
			public int compare(User u1, User u2)
			{
				return u1.age.compareTo(u2.age);
			}
		});

		printList("by Age", users);

		List<User> underageUsers = new ArrayList<User>();
		for (User u : users) {
			if (u.age < 21) {
				underageUsers.add(u);
			}
		}
		printList("Underage users", underageUsers);
	}


	private static void printList(String type, List<User> users)
	{
		System.out.println("Old Way " + type + ":");
		for (User u : users) {
			System.out.println("\t" + u);
		}
		System.out.println();
	}
}
