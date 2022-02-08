import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.util.Enumeration;
import java.util.Hashtable;

public class LoadTable {
	public static void main(String[] args) {
		try {
            FileInputStream fileIn = new FileInputStream("hash.ser");
            ObjectInputStream in = new ObjectInputStream(fileIn);
            Object obj = in.readObject();
            in.close();

            //Ok to suppress this warning as we know the type of serialized objects in the file
            //Another option is to cast to the base Hashtable type instead of the template
            //version. Basically what this is telling us is that the compiler has no way of knowing
            //what is contained in the external data file.
            
            @SuppressWarnings("unchecked")
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
