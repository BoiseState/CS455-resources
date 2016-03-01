package RMISSLex1;

import java.rmi.Remote;
import java.rmi.RemoteException;

import java.util.Date;

/**
 * @author Owner
 */
public interface DateServer extends Remote
{
    public Date getDate() throws RemoteException;
}
