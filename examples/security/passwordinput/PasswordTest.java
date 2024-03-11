package passwordinput;

import java.io.*;
import java.util.Arrays;

/**
 * A simple example to show how to hide password as it is typed.
 * @author amit
 */
public class PasswordTest {
	public static void main(String argv[])
	{

		Console console = System.console();
		String username = console.readLine("Username: ");
		char[] password = console.readPassword("Password: ");

		System.out.println(username + " entered password: " + new String(password));
		// clear the array as soon as the password has been used
		Arrays.fill(password, ' ');
	}
}
