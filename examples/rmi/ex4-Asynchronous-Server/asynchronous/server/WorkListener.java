package asynchronous.server;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface WorkListener extends Remote
{
    public void workCompleted(WorkRequest request, Object result) throws RemoteException;
}
