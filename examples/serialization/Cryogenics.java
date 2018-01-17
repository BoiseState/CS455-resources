import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

/**
 * Experiments with serialization.
 * 
 * Add the serial version UID to the MioAlma class (version 1)
 * 
 * Run and freeze a MioAlma object (version 1).
 * 
 * Refactor MioAlma class to add a gender field. Using this new class, revive an
 * MioAlma version 1 object. Note that the gender field will be set to null.
 * 
 * Run and freeze a MioAlma object (version 2). Refactor MioAlma back to version
 * 1. Revive a MioAlma version 1 object from a freeze-dried MioAlma version 2
 * object!
 * 
 * @author amit
 */
public class Cryogenics
{
    /**
     * Serialize a Mio object.
     */
    private static void freezeDry() {
	MioAlma obj = new MioAlma("Marco Polo", 123456789);
	System.out.println("Serializing object: " + obj);
	try {
	    FileOutputStream fileOut = new FileOutputStream("mio.ser");
	    ObjectOutputStream out = new ObjectOutputStream(fileOut);
	    out.writeObject(obj);
	    out.close();
	} catch (Exception e) {
	    System.out.println(e);
	}
    }


    /**
     * Deserialize a Mio object
     */
    private static void revive() {
	try {
	    FileInputStream fileIn = new FileInputStream("mio.ser");

	    ObjectInputStream in = new ObjectInputStream(fileIn);
	    Object obj = in.readObject();
	    in.close();

	    MioAlma mine = (MioAlma) obj;
	    System.out.println("Deserialized object: " + mine);

	} catch (IOException e) {
	    System.err.println(e);
	} catch (ClassNotFoundException e) {
	    System.err.println(e);
	}
    }


    /**
     * @param args
     */
    public static void main(String[] args) {
	if (args.length < 1) {
	    System.err.println("Usage: Cryogenics <freeze|revive>");
	    System.exit(1);
	}

	if (args[0].equalsIgnoreCase("freeze")) {
	    freezeDry();
	} else if (args[0].equalsIgnoreCase("revive")) {
	    revive();
	}
    }

}
