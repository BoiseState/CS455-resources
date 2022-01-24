import java.util.Random;

public class GenerateRandoms
{ 
    public static void main(String[] args) {
        if (args.length != 1) {
            System.out.println("Usage: java GenerateRandoms <n>");
            System.exit(1);
        }
        int n = Integer.parseInt(args[0]);
        RunExperiment e = new RunExperiment(n);
        e.generateRandoms();
        System.out.printf("%2.2f\n", e.getAverage());
    }
}


class RunExperiment 
{
    private final int RANGE = 100;
    private double average = 0.0;
    private int count;
    
    public RunExperiment(int count) {
        this.count = count;
    }
    
    public void generateRandoms() {
        Random generator = new Random();
        double sum = 0;
        for (int i = 0; i < count; i++)
            sum += generator.nextInt(RANGE); // 0 .. RANGE - 1
        average = sum / count;
    }
    
    public double getAverage() {
        return average;
    }
}
