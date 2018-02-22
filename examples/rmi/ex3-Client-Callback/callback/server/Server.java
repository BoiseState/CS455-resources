package callback.server;

import java.util.Date;

public interface Server extends java.rmi.Remote
{
    /**
     * Returns the current date on the server.
     * @return
     * @throws java.rmi.RemoteException
     */
    Date getDate() throws java.rmi.RemoteException;


    /**
     * Executes the work request on the server and return the result.
     * @param work
     * @return 
     * @throws java.rmi.RemoteException
     */
    Object execute(WorkRequest work) throws java.rmi.RemoteException;


    /**
     * Get a enumeration of strings from the server. 
     * @return
     * @throws java.rmi.RemoteException
     */
    StringEnumeration getList() throws java.rmi.RemoteException;


    /**
     * Server executes the work request and calls back client when done.
     * @param work
     * @param listener
     * @throws java.rmi.RemoteException
     */
    void asyncExecute(WorkRequest work, WorkListener listener) throws java.rmi.RemoteException;
}
