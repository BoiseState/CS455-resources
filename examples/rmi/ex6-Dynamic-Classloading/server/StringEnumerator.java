import java.rmi.*;
import java.util.*;

public class StringEnumerator 
	extends java.rmi.server.UnicastRemoteObject implements StringEnumeration {

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
