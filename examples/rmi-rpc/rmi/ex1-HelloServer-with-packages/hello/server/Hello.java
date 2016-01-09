package hello.server;


public interface Hello extends java.rmi.Remote {
        String sayHello() throws java.rmi.RemoteException;
}
