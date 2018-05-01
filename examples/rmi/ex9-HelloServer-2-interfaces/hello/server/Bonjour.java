package hello.server;

public interface Bonjour extends java.rmi.Remote
{
    String disBonjour() throws java.rmi.RemoteException;
}
