import java.awt.Color;
import java.lang.reflect.Field;
import java.lang.reflect.Method;


/**
 * The Color class reflects on itself.
 * @author amit
 *
 */
public class SelfReflect {
	public static void main(String [] args) 
	{
		Class<Color> type = Color.class;
		
		Method [] methods = type.getMethods();
		for (int i=0; i< methods.length; i++) 
			System.out.println( methods[i]);

		Field [] fields = type.getFields();
		for (int i=0; i< fields.length; i++) 
			System.out.println( fields[i]);
	}
}
