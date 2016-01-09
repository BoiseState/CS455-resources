
/*package classExamples.io;*/
import java.io.*;
import java.util.*;
import java.awt.*;

public class SaveTable {
	public static void main (String [] args) {
		Hashtable<String, Object> h = new Hashtable<String, Object>(); 
		// Hashtable implements the serializable interface
		// so we can try to  freeze dry it
		h.put("string","Cyrano De Bergerac");
		h.put("int",new Integer("1600"));
		h.put("double", new Double(Math.PI));
		h.put("color", new Color(255,0,0));

		try {
			FileOutputStream fileout = new FileOutputStream("hash.serial");
			ObjectOutputStream out = new ObjectOutputStream(fileout);
			out.writeObject(h);
		}
		catch (Exception e) {
			System.out.println(e);
		}
	}
}	



