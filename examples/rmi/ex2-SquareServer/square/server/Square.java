package square.server;

public interface Square extends java.rmi.Remote
{
    int square(int arg) throws java.rmi.RemoteException;
}
