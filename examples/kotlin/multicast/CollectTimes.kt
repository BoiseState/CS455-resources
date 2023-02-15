import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress
import java.net.InetSocketAddress
import java.net.MulticastSocket
import java.net.NetworkInterface
import java.net.SocketTimeoutException
import java.time.LocalDateTime
import java.time.format.DateTimeFormatter
import kotlin.system.exitProcess

fun main(args: Array<String>) {
    val sizeMap = mapOf("collector" to 4, "provider" to 5)
    if (args.isEmpty() || args.size != sizeMap[args[0]]) {
        println("java collector <udp_port> <mcast_address> <mcast_port>")
        println("java provider <udp_port> <mcast_address> <mcast_port> <net_iface>")
        exitProcess(1)
    }
    val skt = DatagramSocket(args[1].toInt())
    val mcastAddr = InetAddress.getByName(args[2])
    val mcastPort = args[3].toInt()
    when (args[0]) {
        "collector" -> collector(mcastAddr, mcastPort, skt)
        "provider" -> {
            val ni = NetworkInterface.getByName(args[4])
            if (ni === null) {
                println("could not find network interface '${args[4]}'")
                exitProcess(1)
            }
            provider(mcastAddr, mcastPort, skt, ni)
        }
    }
}

fun collector(mcastAddr: InetAddress, mcastPort: Int, skt: DatagramSocket) {
    skt.soTimeout = 5000
    val msg = "get_time".toByteArray()
    val pkt = DatagramPacket(msg, msg.size, mcastAddr, mcastPort)
    skt.send(pkt)
    val buf = ByteArray(1024)
    val recv = DatagramPacket(buf, buf.size)
    try {
        while (true) {
            skt.receive(recv)
            println("from: ${recv.socketAddress}: ${String(recv.data)}")
        }
    } catch (_: SocketTimeoutException) {}
    println("finished collecting responses")
}

fun provider(mcastAddr: InetAddress, mcastPort: Int, skt: DatagramSocket, netIface: NetworkInterface) {
    val mcast = MulticastSocket(mcastPort)
    mcast.joinGroup(InetSocketAddress(mcastAddr, mcastPort), netIface)
    val buf = ByteArray(1024)
    val recv = DatagramPacket(buf, buf.size)
    while (true) {
        println("waiting to receive data")
        mcast.receive(recv)
        val cmd = String(recv.data, 0, recv.length)
        if (cmd != "get_time") {
            println("unsupported command \"$cmd\"")
            continue
        }
        println("got time request from ${recv.socketAddress}")
        val date = LocalDateTime.now()
        val resp = DateTimeFormatter.ofPattern("uuuu/MM/dd HH:mm:ss").format(date).toByteArray()
        skt.send(DatagramPacket(resp, resp.size, recv.socketAddress))
    }
}
