import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.List;

public interface Service extends Remote
{
    Doodle execute(Doodle dood) throws RemoteException;
}
