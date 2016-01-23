import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.util.Enumeration;
import java.util.Hashtable;

public class LoadTable
{
    public static void main(String[] args)
    {
	try {
	    FileInputStream fileIn = new FileInputStream("hash.serial");
	    ObjectInputStream in = new ObjectInputStream(fileIn);
	    Object obj = in.readObject();
	    in.close();

	    Hashtable<String, Object> h = (Hashtable<String, Object>) obj;
	    System.out.println(h.toString());

	    Enumeration<Object> list = h.elements();
	    while (list.hasMoreElements()) {
		System.out.println(list.nextElement());
	    }

	} catch (FileNotFoundException e) {
	    System.err.println(e);
	} catch (IOException e) {
	    System.err.println(e);
	} catch (ClassNotFoundException e) {
	    System.err.println(e);
	}
    }
}
