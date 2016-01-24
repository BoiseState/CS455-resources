import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

/**
 * Demo how to freeze dry and revive objscts.
 * 
 * @author amit
 */
public class Cryogenics
{
    private static void freezeDry()
    {
	Mio obj = new Mio("Marco Polo", 123456789);
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

    private static void revive()
    {	
	try {
	    FileInputStream fileIn = new FileInputStream("mio.ser");

	    ObjectInputStream in = new ObjectInputStream(fileIn);
	    Object obj = in.readObject();
	    in.close();
	    
	    Mio mine = (Mio) obj;
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
    public static void main(String[] args)
    {
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
