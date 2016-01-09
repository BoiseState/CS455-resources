package clientcallback.server;

import java.util.Date;


public interface Server extends java.rmi.Remote 
{
    Date getDate() throws java.rmi.RemoteException;
    Object execute( WorkRequest work ) throws java.rmi.RemoteException;
	StringEnumeration getList() throws java.rmi.RemoteException;
    void asyncExecute( WorkRequest work, WorkListener listener ) throws java.rmi.RemoteException;
}

