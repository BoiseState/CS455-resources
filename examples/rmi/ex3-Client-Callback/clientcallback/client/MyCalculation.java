package clientcallback.client;

import clientcallback.server.WorkRequest;

public class MyCalculation extends WorkRequest
{

    private static final long serialVersionUID = -6592375958392734131L;
    int n;


    public MyCalculation(int n) {
	this.n = n;
    }


    public Object execute() {
	return new Integer(n * n);
    }
}
