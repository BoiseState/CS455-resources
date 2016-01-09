package asynchronous.server;

import java.rmi.RemoteException;


public class StringEnumerator 
	extends java.rmi.server.UnicastRemoteObject implements StringEnumeration {

	private static final long serialVersionUID = 2131129531057862312L;
	String [] list;
	int index = 0;
 
    public StringEnumerator( String [] list ) throws RemoteException { 
		this.list = list;
	}
	public boolean hasMoreItems() {
		return index < list.length;
	}
	public String nextItem() {
		return list[index++];
	}
}
