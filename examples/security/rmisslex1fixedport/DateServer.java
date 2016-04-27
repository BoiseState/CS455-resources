package rmisslex1fixedport;

import java.rmi.Remote;
import java.rmi.RemoteException;

import java.util.Date;

/**
 * @author amit
 */
public interface DateServer extends Remote
{
    /**
     * Get the current date and time object from the server.
     * @return the date on the server
     * @throws RemoteException
     */
    public Date getDate() throws RemoteException;
}
