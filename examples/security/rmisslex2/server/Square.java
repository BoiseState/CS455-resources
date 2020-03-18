package rmisslex2.server;

public interface Square extends java.rmi.Remote
{
    /**
     * Remotely calculate the square of the provided value.
     * @param arg
     * @return
     * @throws java.rmi.RemoteException
     */
    int square(int arg) throws java.rmi.RemoteException;
}
