package hello.server;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class HelloServer extends UnicastRemoteObject implements Hello
{
    private static final long serialVersionUID = -427480263966725427L;
    private String name;
    
    public HelloServer(String s) throws RemoteException {
  	super();
  	name = s;
      }

      public String sayHello() throws RemoteException {
  	return "Hello World!";
      }
}
