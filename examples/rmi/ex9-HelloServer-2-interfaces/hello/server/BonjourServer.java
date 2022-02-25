package hello.server;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class BonjourServer extends UnicastRemoteObject implements Bonjour
{
    private static final long serialVersionUID = -8937780426086065248L;
    private String name;

    public BonjourServer(String s) throws RemoteException {
        super();
        name = s;
        System.out.println("BonjourServer: " + name + " server created");

    }


    public String disBonjour() throws RemoteException {
        return "Bonjour le monde!";
    }
}