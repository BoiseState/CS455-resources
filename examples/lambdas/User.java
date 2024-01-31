public class User {
	protected Integer id;
	protected String firstName;
	protected String lastName;
	protected Integer age;

	public User() {
	}


	public User(int id, String first, String last, int age) {
		this.id = id;
		this.firstName = first;
		this.lastName = last;
		this.age = age;
	}


	public String toString()
	{
		return "" + id + ", " + firstName + ", " + lastName + ", " + age;
	}
}
