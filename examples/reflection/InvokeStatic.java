import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

/**
 * InvokeStatic a static method without any arguments on a specified class by
 * name. For example: java InvokeStatic java.lang.Math random
 */
public class InvokeStatic
{
    public static void main(String[] args) {
	if (args.length == 0) {
	    System.err.println("Usage: java InvokeStatic <class name> <static method name>");
	    System.exit(1);
	}
	try {
	    Class<?> c = Class.forName(args[0]);
	    /* Method m = c.getMethod(args[1], new Class[]{}); */
	    /* Object ret = m.invoke(null, (Object[])null); */
	    /* Can use the following because of varargs */
	    Method m = c.getMethod(args[1]);
	    Object ret = m.invoke(null);
	    System.out.println(
	            "Invoked static method: " + args[1] + " of class: " + args[0] + " with no args\n Results: " + ret);
	} catch (ClassNotFoundException e1) {
	    // cannot find the class
	    System.out.println(e1);
	} catch (NoSuchMethodException e2) {
	    // that method doesn't exist
	    System.out.println(e2);
	} catch (IllegalAccessException e3) {
	    // don't have permission to InvokeStatic that method
	    System.out.println(e3);
	} catch (InvocationTargetException e4) {
	    // an exception occurred while invoking that method
	    System.out.println("Method threw an: " + e4.getTargetException());
	}
    }
}
