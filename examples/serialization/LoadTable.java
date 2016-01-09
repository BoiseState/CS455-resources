
/*package classExamples.io;*/
import java.io.*;
import java.util.*;

public class LoadTable {
	public static void main (String [] args) {
		try {
			FileInputStream fileIn = new FileInputStream("hash.serial");
			ObjectInputStream in = new ObjectInputStream(fileIn);
			Object obj = in.readObject();
			Hashtable h = (Hashtable) obj;
			System.out.println(h.toString());

			Enumeration list = h.elements();
			while (list.hasMoreElements())
			{
				System.out.println(list.nextElement());
			}
		}
		catch (Exception e) {
			System.out.println(e);
		}
	}
}	



