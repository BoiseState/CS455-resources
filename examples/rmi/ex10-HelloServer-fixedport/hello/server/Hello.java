package hello.server;

/**
 * A remote interface to demonstrate RMI.
 *
 */
public interface Hello extends java.rmi.Remote
{
    String sayHello() throws java.rmi.RemoteException;
}
