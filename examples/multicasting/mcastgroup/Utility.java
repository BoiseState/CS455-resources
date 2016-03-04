import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;


/**
 * @author amit
 *
 */
public class Utility 
{
	
	/**
	 * Convert an int to an array of bytes
	 * @param integer
	 * @return
	 * @throws IOException
	 */
	public static byte[] getBytes (final int integer) 
	throws IOException {	
	      ByteArrayOutputStream bos = new ByteArrayOutputStream();
	      DataOutputStream dos = new DataOutputStream(bos);
	      dos.writeInt(integer);
	      dos.flush();
	      return bos.toByteArray();
	}
	
	/**
	 * @param arr
	 * @return
	 * @throws IOException
	 */
	public static int getInt (final byte[] arr) 
	throws IOException {	
	      ByteArrayInputStream bis = new ByteArrayInputStream(arr);
	      DataInputStream dis = new DataInputStream(bis);
	      int result = dis.readInt();
	      dis.close();
	      bis.close();
	      return result;
	}
	
	
	/**
	 * Converts a serialized object to an array of bytes.
	 * This entire method came from www.sun.com and http://www.javafaq.nu/java-article236.html
	 * @param obj the object to be converted
	 * @return the object as a byte array
	 * @throws java.io.IOException
	 */
	public static byte[] getBytes(Object obj) throws java.io.IOException {
	      ByteArrayOutputStream bos = new ByteArrayOutputStream();
	      ObjectOutputStream oos = new ObjectOutputStream(bos);
	      oos.writeObject(obj);
	      oos.flush();
	      oos.close();
	      bos.close();
	      byte [] data = bos.toByteArray();
	      return data;
	}

	/**
	 * Converts a byte array to an Object
	 * @param data the array of bytes to be converted
	 * @return the resulting object
	 * @throws java.io.IOException
	 */
	public static Object getObject(byte[] data) throws java.io.IOException {
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
