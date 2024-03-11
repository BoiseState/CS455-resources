import java.awt.Color;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.util.HashMap;

/**
 * Utility functions to support converting objects to bytes and vice versa.
 * 
 * @author amit
 *
 */
public class Utility {

	/**
	 * Converts a serialized object to an array of bytes.
	 * 
	 * @param obj the object to be converted
	 * @return the object as a byte array
	 * @throws java.io.IOException
	 */
	public static byte[] getBytes(Object obj) throws java.io.IOException
	{
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		ObjectOutputStream oos = new ObjectOutputStream(bos);
		oos.writeObject(obj);
		oos.flush();
		oos.close();
		bos.close();
		byte[] data = bos.toByteArray();
		return data;
	}


	/**
	 * Converts a byte array to an Object.
	 * 
	 * @param data the array of bytes to be converted
	 * @return the resulting object
	 * @throws java.io.IOException
	 */
	public static Object getObject(byte[] data) throws java.io.IOException
	{
		ByteArrayInputStream bis = new ByteArrayInputStream(data);
		ObjectInputStream ois = new ObjectInputStream(bis);
		Object o = null;
		try {
			o = ois.readObject();
		} catch (ClassNotFoundException e) {
			System.err.println(e);
			e.printStackTrace();
		}
		return o;
	}
	
	public static void main (String[] args) throws IOException
	{
		String test1 = "Marco Polo";
		
		HashMap<String, Color> test2 = new HashMap<String, Color>();
		test2.put("red", new Color(255, 0, 0));
		test2.put("green", new Color(0, 255, 0));
		test2.put("blue", new Color(0, 0,255));
		
		byte[] bytes = getBytes(test1);
		System.out.println(getObject(bytes));
		
		byte[] hashbytes = getBytes(test2);
		System.out.println(getObject(hashbytes));
		
		
	}

}
