
/**
 * Trace the ancestry of a specified class name.
 * 
 * @author amit
 *
 */
public class TraceAncestry
{

    /**
     * @param args
     */
    public static void main(String[] args) {
	if (args.length == 0) {
	    System.err.println("Usage: java TraceAncestry <class name>");
	    System.exit(1);
	}

	Class<?> type = null;
	try {
	    type = Class.forName(args[0]);
	} catch (ClassNotFoundException e) {
	    System.err.println(e);
	}

	System.out.println("class " + type.getSimpleName());
	Class<?> superclass = type;
	do {
	    superclass = superclass.getSuperclass();
	    if (superclass == null) break;
	    System.out.println("extends " + superclass.getCanonicalName());
	} while (!superclass.getCanonicalName().equals("java.lang.Object"));
    }
}
