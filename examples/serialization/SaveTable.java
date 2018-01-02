import java.awt.Color;
import java.io.FileOutputStream;
import java.io.ObjectOutputStream;
import java.util.Hashtable;

public class SaveTable {
	public static void main(String[] args) {
		Hashtable<String, Object> h = new Hashtable<String, Object>();
		// Hashtable implements the serializable interface so it can be freeze dried
		h.put("string", "Cyrano De Bergerac");
		h.put("int", new Integer("1619"));
		h.put("double", new Double(Math.PI));
		h.put("color", new Color(255, 0, 0));

		try {
			FileOutputStream fileout = new FileOutputStream("hash.ser");
			ObjectOutputStream out = new ObjectOutputStream(fileout);
			out.writeObject(h);
			out.close();
		} catch (Exception e) {
			System.out.println(e);
		}
	}
}
