import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Service extends Remote
{
    Doodle execute(Doodle dood) throws RemoteException;
}
