package synchronous.client;

import synchronous.server.WorkRequest;

/**
 * A class representing a work request.
 */
public class MyCalculation extends WorkRequest
{
    private static final long serialVersionUID = -6592375958392734131L;
    int n;

    public MyCalculation(int n) {
        this.n = n;
    }


    public Object execute() {
        return n * n;
    }
}
