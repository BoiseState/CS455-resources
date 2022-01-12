import java.util.ArrayList;

/*
 * A simple shared queue that is synchronized for use by multiple threads.
 * @author amit
 */

public class SharedQueue
{
    private ArrayList<String> queue;
    private int maxSize;

    public SharedQueue(int maxSize) {
        queue = new ArrayList<String>();
        this.maxSize = maxSize;
    }


    public synchronized int size() {
        return queue.size();
    }


    public synchronized void putMessage(String msg) throws InterruptedException {
        while (this.size() == maxSize)
            wait();
        queue.add(msg);
        notifyAll(); // for multiple consumers
    }


    public synchronized String getMessage() throws InterruptedException {
        notifyAll(); // use notify for single producer
        while (this.size() == 0)
            wait();
        String message = queue.remove(0); // first element
        queue.remove(message);
        return message;
    }

}
