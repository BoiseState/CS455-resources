import java.net.*
import kotlin.system.exitProcess

class UdpServer(port: Int) {
    private val skt = DatagramSocket(port)
    private var count = 0
    // Runs as part of the constructor
    init {
        Runtime.getRuntime().addShutdownHook(Thread {
            println("Number of datagrams received by server: ${this.count}")
        })
    }

    fun run() = try {
        while (true) {
            val recv = DatagramPacket(ByteArray(1024), 1024)
            this.skt.receive(recv)
            val echo = "Echo! " + String(recv.data)
            val send = DatagramPacket(echo.toByteArray(), 1024, recv.socketAddress)
            this.skt.send(send)
        }
    } catch(e: Exception) {
        println(e)
    }
}

fun callServer(host: String, port: Int, msg: String, sendPort: Int) {
    val skt = DatagramSocket(sendPort)
    val addr = InetSocketAddress(host, port)
    val pkt = DatagramPacket(msg.toByteArray(), msg.length, addr)
    skt.send(pkt)
    val recv = DatagramPacket(ByteArray(1024), 1024)
    println("waiting for receive on port $sendPort")
    // UDP is a connectionless protocol. Try running the client without having a
    // server up, and point another client to send to this process's sendPort
    // to trigger the error condition below. The receive method doesn't branch
    // off into a different connection like TCP ServerSocket does.
    skt.receive(recv)
    if (recv.socketAddress != addr) {
        println("Expected recv from $addr, but got from ${recv.socketAddress}")
        return
    }
    println("response from server: '${String(recv.data)}'")
}

fun main(args: Array<String>) {
    if (
        args.size !in setOf(2, 5) ||
        args[0] !in setOf("client", "server") ||
        args.size == 2 && args[0] == "client" ||
        args.size == 5 && args[0] == "server"
    ) {
        println("""
        Usage:
            EchoUdp server <port>
            EchoUdp client <host> <port> <message> <send_port>
        """.trimIndent())
        exitProcess(1)
    }
    when (args[0]) {
        "client" -> callServer(args[1], args[2].toInt(), args[3], args[4].toInt())
        "server" -> UdpServer(args[1].toInt()).run()
    }
}