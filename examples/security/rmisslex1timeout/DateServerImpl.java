package rmisslex1timeout;

import java.io.IOException;
import java.net.Socket;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.RMIClientSocketFactory;
import java.rmi.server.RMIServerSocketFactory;
import java.rmi.server.RemoteServer;
import java.rmi.server.ServerNotActiveException;
import java.rmi.server.UnicastRemoteObject;
import javax.rmi.ssl.SslRMIClientSocketFactory;
import javax.rmi.ssl.SslRMIServerSocketFactory;

import java.util.Date;

public class DateServerImpl implements DateServer
{
    public DateServerImpl() throws RemoteException {
    }


    public void bind(String name) {
        try {
            RMIClientSocketFactory rmiClientSocketFactory = new TimedSslRMIClientSocketFactory();
            RMIServerSocketFactory rmiServerSocketFactory = new SslRMIServerSocketFactory();
            DateServer ccAuth = (DateServer) UnicastRemoteObject.exportObject(this, 0, rmiClientSocketFactory,
                    rmiServerSocketFactory);
            Registry registry = LocateRegistry.createRegistry(2004);
            registry.rebind(name, ccAuth);
            System.out.println(name + " bound in registry");
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("Exception occurred: " + e);
        }
    }


    public Date getDate() throws RemoteException {
        try {
            System.out.println("Connect from: " + RemoteServer.getClientHost());
        } catch (ServerNotActiveException e) {
            System.err.println(e);
        }
        try {
            Thread.sleep(10000); // simulate a slow server
        } catch (InterruptedException e) {
            System.err.println(e);
        }
        return new java.util.Date();
    }


    public static void main(String args[]) {
        System.out.println("Setting System Properties....");
        System.setProperty("javax.net.ssl.keyStore", "rmisslex1timeout/resources/Server_Keystore");
        // Warning: change to match your password! Also the password should be
        // stored encrypted in a file outside the program.
        System.setProperty("javax.net.ssl.keyStorePassword", "test123");
        System.setProperty("java.security.policy", "rmisslex1timeout/resources/mysecurity.policy");
        try {
            DateServerImpl server = new DateServerImpl();
            server.bind("DateServerImpl");
        } catch (Throwable th) {
            th.printStackTrace();
            System.out.println("Exception occurred: " + th);
        }
    }
}

class TimedSslRMIClientSocketFactory extends SslRMIClientSocketFactory
{
    // private static final long serialVersionUID = 7280036713852974589L;

    public Socket createSocket(String host, int port) throws IOException {
        Socket s = super.createSocket(host, port);
        s.setSoTimeout(2000); // ms
        return s;
    }
}
