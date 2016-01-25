package asynchronous.server;

import java.rmi.Remote;
import java.rmi.RemoteException;


public interface StringEnumeration extends Remote {
	public boolean hasMoreItems() throws RemoteException;
	public String nextItem() throws RemoteException;
}

