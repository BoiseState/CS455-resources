
/**
 * Shows how to create threads by implementing Runnable interface.
 */
class AnotherElement implements Runnable
{
    private String name;

    public AnotherElement(String s)
    {
	name = s;
    }

    public String getName()
    {
	return name;
    }

    public void run()
    {
	for (int i = 0; i < 10; i++) {
	    System.out.println("This is the " + this.getName() + " thread.");
	}
    }
}

public class RunnableExample
{
    public static void main(String args[])
    {
	AnotherElement g1 = new AnotherElement("water");
	AnotherElement g2 = new AnotherElement("fire");
	AnotherElement g3 = new AnotherElement("earth");
	AnotherElement g4 = new AnotherElement("air");
	AnotherElement g5 = new AnotherElement("void");

	new Thread(g1).start();
	new Thread(g2).start();
	new Thread(g3).start();
	new Thread(g4).start();
	new Thread(g5).start();
    }
}
