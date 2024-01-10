
/**
 * This is the canonical example of a simple calculation to use for distributed systems examples!
 * 
 */
public class MyCalculation implements Runnable
{
    private int[] values;

    /**
     * @param n      How many values to calculate.
     */
    public MyCalculation(int n) {
        values = new int[n];
    }


    /**
     * {@inheritDoc}
     */
    public void run() {
        for (int i = 0; i < values.length; i++)
            values[i] = i * i;
    }


    /**
     * @return the array of calculated values
     */
    public int[] getValues() {
        return values;
    }
}
