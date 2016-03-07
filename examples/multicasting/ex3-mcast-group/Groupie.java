
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.NetworkInterface;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;
import java.util.Collections;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

/**
 * An incomplete example of groups using multicast socket.
 * 
 * @author amit
 */
public class Groupie
{

    private static final int debug = 1;
    private static final int discoveryPort = 5000;
    private static final int timeout = 5000; // milliseconds
    private static final int default_timeout = 0; // infinite
    private static final String mcastAddress = "228.1.1.1";

    private static final int DISCOVER_GROUP = 1;
    private static final int LEAVE_GROUP = 2;
    private static final int JOIN_GROUP = 3;
    private static final int I_AM_HERE = 4;

    private Set<InetAddress> servers = Collections.synchronizedSet(new HashSet<InetAddress>());
    private InetAddress group;
    private MulticastSocket s;


    public Groupie(InetAddress group, MulticastSocket s) {
	this.s = s;
	this.group = group;
	Runtime sys = Runtime.getRuntime();
	Thread CleanupThread = new CleanupThread(this);
	sys.addShutdownHook(CleanupThread);
    }


    private void printDatagram(DatagramPacket pkt) throws UnknownHostException, IOException {
	String packetType = "";
	switch (Utility.getInt(pkt.getData())) {
	    case 1:
		packetType = "DISCOVER_GROUP";
		break;
	    case 2:
		packetType = "LEAVE_GROUP";
		break;
	    case 3:
		packetType = "JOIN_GROUP";
		break;
	    case 4:
		packetType = "I_AM_HERE";
		break;
	    default:
		break;
	}
	System.out.println("Server " + InetAddress.getLocalHost() + ": Received packet type " + packetType + " from "
	        + pkt.getAddress());
    }


    private void initialize() throws IOException {
	int groupSize = 0;

	byte[] buf = new byte[4];
	DatagramPacket recv = new DatagramPacket(buf, buf.length, group, discoveryPort);
	servers.add(InetAddress.getLocalHost());
	System.out.println("In groupDance");

	try {
	    s.setSoTimeout(timeout);
	    DatagramPacket hello = new DatagramPacket(Utility.getBytes(DISCOVER_GROUP), 4, group, discoveryPort);
	    s.send(hello);

	    s.receive(recv);
	    printDatagram(recv);
	    if (Utility.getInt(buf) == LEAVE_GROUP) {
		servers.remove(recv.getAddress());
		if (debug == 1) printGroup();
	    } else if (!recv.getAddress().equals(InetAddress.getLocalHost())) {
		if (Utility.getInt(buf) == DISCOVER_GROUP) {
		    DatagramPacket ack = new DatagramPacket(Utility.getBytes(I_AM_HERE), 4, group, discoveryPort);
		    s.send(ack);
		    if (!servers.contains(recv.getAddress())) servers.add(recv.getAddress());
		    if (debug == 1) printGroup();

		} else if (Utility.getInt(buf) == I_AM_HERE) {
		    if (!servers.contains(recv.getAddress())) servers.add(recv.getAddress());
		    if (debug == 1) {
			System.err.println("Found a group");
			printGroup();
		    }
		}
	    } else {
		s.setSoTimeout(timeout); // try again
		s.receive(recv);
		printDatagram(recv);
		if (Utility.getInt(buf) == LEAVE_GROUP) {
		    servers.remove(recv.getAddress());
		    if (debug == 1) printGroup();
		} else if (Utility.getInt(buf) == DISCOVER_GROUP) {
		    DatagramPacket ack = new DatagramPacket(Utility.getBytes(I_AM_HERE), 4, group, discoveryPort);
		    s.send(ack);
		    if (!servers.contains(recv.getAddress())) servers.add(recv.getAddress());
		    if (debug == 1) printGroup();
		} else if (Utility.getInt(buf) == I_AM_HERE) {
		    if (!servers.contains(recv.getAddress())) servers.add(recv.getAddress());
		    if (debug == 1) {
			System.err.println("Found a group");
			printGroup();
		    }
		}
	    }
	} catch (SocketTimeoutException e) {
	    // no one responded, so start a new group
	    s.setSoTimeout(default_timeout);
	    if (debug == 1) {
		System.err.println("No response for discover group. Starting a new group");
		printGroup();
	    }
	}
	s.setSoTimeout(default_timeout);
    }


    public void leave() {
	try {
	    DatagramPacket bye = new DatagramPacket(Utility.getBytes(LEAVE_GROUP), 4, group, discoveryPort);
	    s.send(bye);
	    servers.clear();
	    s.leaveGroup(group);
	} catch (IOException e) {
	    System.err.println(e);
	}
    }


    private void groupDance() throws IOException {
	byte[] buf = new byte[4];
	DatagramPacket recv = new DatagramPacket(buf, buf.length, group, discoveryPort);
	System.out.println("In groupDance");

	while (true) {
	    s.receive(recv);
	    printDatagram(recv);
	    if (!recv.getAddress().equals(InetAddress.getLocalHost())) {
		if (Utility.getInt(buf) == DISCOVER_GROUP) {
		    DatagramPacket ack = new DatagramPacket(Utility.getBytes(I_AM_HERE), 4, group, discoveryPort);
		    s.send(ack);
		    if (!servers.contains(recv.getAddress())) servers.add(recv.getAddress());
		    if (debug == 1) printGroup();
		} else if (Utility.getInt(buf) == JOIN_GROUP) {
		    if (!servers.contains(recv.getAddress())) servers.add(recv.getAddress());
		    if (debug == 1) printGroup();
		} else if (Utility.getInt(buf) == LEAVE_GROUP) {
		    servers.remove(recv.getAddress());
		    if (debug == 1) printGroup();
		} else if (Utility.getInt(buf) == I_AM_HERE) {
		    if (!servers.contains(recv.getAddress())) servers.add(recv.getAddress());
		    if (debug == 1) printGroup();
		}
	    }
	}
    }


    private void printGroup() {
	Iterator itr = servers.iterator();
	while (itr.hasNext()) {
	    System.err.println(itr.next());
	}

    }


    /**
     * @param args
     */
    public static void main(String[] args) {

	String networkInterface;
	if (args.length != 1) {
	    System.err.println("Usage: java Groupie <network-interface>");
	    System.exit(1);
	}
	networkInterface = args[0];

	try {
	    InetAddress group = InetAddress.getByName(mcastAddress);
	    MulticastSocket s = new MulticastSocket(discoveryPort);
	    NetworkInterface net = NetworkInterface.getByName(networkInterface);
	    s.setNetworkInterface(net);
	    s.joinGroup(group);
	    Groupie g = new Groupie(group, s);
	    g.initialize();
	    g.groupDance();

	} catch (UnknownHostException e) {
	    System.err.println(e);
	    System.exit(1);
	} catch (IOException e) {
	    System.err.println(e);
	    System.exit(1);
	}

    }

}

class CleanupThread extends Thread
{
    Groupie server;


    public CleanupThread(Groupie server) {
	this.server = server;
    }


    public void run() {
	System.out.println("Groupie server shutting down...");
	server.leave();
    }

}
