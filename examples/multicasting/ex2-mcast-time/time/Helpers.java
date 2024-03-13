package time;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

/**
 * Helper class that provides functions that convert an Object to bytes[] and
 * vice-versa.
 * 
 */
public class Helpers {
	/**
	 * Converts a serialized object to an array of bytes.
	 * 
	 * @param obj
	 *            the object to be converted
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
	 * Converts a byte array to an Object
	 * 
	 * @param data
	 *            the array of bytes to be converted
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
}
