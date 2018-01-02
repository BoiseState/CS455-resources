import java.lang.reflect.Field;
import java.lang.reflect.Method;

/**
 * Print all public methods and fields for a user specified class name.
 */
public class ReflectOnAnother {
	public static void main(String[] args) {
		if (args.length == 0) {
			System.err.println("Usage: java ReflectOnAnother <class name>");
			System.exit(1);
		}

		Class<?> type = null;
		try {
			type = Class.forName(args[0]);
		} catch (ClassNotFoundException e) {
			System.err.println(e);
		}

		Method[] methods = type.getMethods();
		for (Method m : methods)
			System.out.println("  " + m);

		Field[] fields = type.getFields();
		for (Field f : fields)
			System.out.println("  " + f);
	}
}
