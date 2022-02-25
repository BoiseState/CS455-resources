import java.rmi.Remote;
import java.rmi.RemoteException;

/**
 * Interface for the RMIThreadServer object, which templates the calls we are
 * exposing over RMI
 */

public interface RMIThreadServer extends Remote
{
    public void update() throws RemoteException;

    public int read() throws RemoteException;
}
