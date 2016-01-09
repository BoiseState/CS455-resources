import java.rmi.*;

public interface StringEnumeration extends Remote {
	public boolean hasMoreItems() throws RemoteException;
	public String nextItem() throws RemoteException;
}

