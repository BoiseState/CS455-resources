import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

/**
 * InvokeDynamic shows how to invoke a method dynamically.
 */
public class InvokeDynamic {
	public static void main(String[] args) throws Throwable {
		String str = "java.lang.reflect";
		Throwable failure = null;
		try {
			Method indexM = String.class.getMethod("indexOf", String.class, int.class);
			System.out.println(indexM.invoke(str, ".", 8));
		} catch (NoSuchMethodException e) {
			failure = e;
		} catch (IllegalAccessException e) {
			failure = e;
		} catch (InvocationTargetException e) {
			failure = e;
		}
		if (failure != null)
			throw failure;
	}
}
