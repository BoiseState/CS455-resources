package synchronous.server;

import java.util.Date;

public interface Server extends java.rmi.Remote
{
    /**
     * Returns the current date on the server.
     * 
     * @return
     * @throws java.rmi.RemoteException
     */
    Date getDate() throws java.rmi.RemoteException;


    /**
     * Executes the work request on the server and return the result.
     * 
     * @param work
     * @return
     * @throws java.rmi.RemoteException
     */
    Object execute(WorkRequest work) throws java.rmi.RemoteException;


    /**
     * Get a enumeration of strings from the server.
     * 
     * @return
     * @throws java.rmi.RemoteException
     */
    StringEnumerationRequest getList() throws java.rmi.RemoteException;
}
