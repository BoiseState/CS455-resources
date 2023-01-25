import java.io.EOFException
import java.io.IOException
import java.io.ObjectInputStream
import java.io.ObjectOutputStream
import java.net.ServerSocket
import java.net.Socket
import java.util.*
import java.util.concurrent.Executors
import java.util.concurrent.Semaphore
import kotlin.system.exitProcess

class ObjSocket(private val skt: Socket) {
    private val oout = ObjectOutputStream(skt.getOutputStream())
    private val oin = ObjectInputStream(skt.getInputStream())
    val remoteAddr = skt.remoteSocketAddress
    fun read(): Any = oin.readObject()
    fun write(obj: Any) {
        oout.writeObject(obj)
        oout.flush()
    }
    fun close() {
        oout.close()
        oin.close()
        skt.close()
    }
}

fun serverSingleThread(port: Int)  {
    try {
        val skt = ServerSocket(port)
        while (true) {
            val clientSkt = ObjSocket(skt.accept())
             serveClient(clientSkt)
        }
    } catch (e: Exception) {
        println(e)
    }
}

fun serverMultiThreaded(port: Int)  {
    try {
        val skt = ServerSocket(port)
        while (true) {
            val clientSkt = ObjSocket(skt.accept())
            val t = Thread { serveClient(clientSkt) }
            // We want client threads to be lower priority than the main
            // server thread. To avoid possible starvation of the server
            // thread in case of high load, so we aren't rejecting new
            // connections.
            t.priority = Thread.currentThread().priority - 1
            t.start()
        }
    } catch (e: Exception) {
        println(e)
    }
}

fun serverLimitedThreads(port: Int)  {
    try {
        val sem = Semaphore(2)
        val skt = ServerSocket(port)
        while (true) {
            sem.acquire()
            val clientSkt = ObjSocket(skt.accept())
            val t = Thread {
                serveClient(clientSkt)
                sem.release()
            }
            t.priority = Thread.currentThread().priority - 1
            t.start()
        }
    } catch (e: Exception) {
        println(e)
    }
}

fun serverThreadPool(port: Int)  {
    try {
        val pool = Executors.newFixedThreadPool(2)
        val clientPriority = Thread.currentThread().priority - 1
        val skt = ServerSocket(port)
        while (true) {
            val clientSkt = ObjSocket(skt.accept())
            pool.execute {
                Thread.currentThread().priority = clientPriority
                serveClient(clientSkt)
            }
        }
    } catch (e: Exception) {
        println(e)
    }
}

fun serveClient(skt: ObjSocket) {
    try {
        println("serving client from ${skt.remoteAddr}")
        while (true) {
            val echo = try {
                skt.read() as String
            } catch (e: EOFException) {
                // this exception signifies that the client has severed
                // the connection
                break
            }
            skt.write("Echo! $echo")
        }
        println("finished serving client ${skt.remoteAddr}")
    } catch (e: InterruptedException) {
        println(e)
    } catch (e: IOException) {
        println(e)
    }
}

fun client(skt: ObjSocket, msg: String, iters: Int, delayMS: Long) {
    try {
        var i = 0
        while (iters == 0 || i < iters) {
            skt.write("$msg #$i")
            val echo = skt.read() as String
            // my terminal seems to print the string and newline in two separate
            // flushes, which leads to weird looking output when I'm testing the
            // multithreaded server using the script. Including the newline here
            // explicitly fixes the output.
            print("received echo from server: $echo\n")
            i += 1
            if (delayMS > 0 && i < iters) {
                Thread.sleep(delayMS)
            }
        }
    } catch (e: InterruptedException) {
        println(e)
    } catch (e: IOException) {
        println(e)
    }
}

fun server(port: Int, mode: String) {
    when (mode) {
        "single" -> serverSingleThread(port)
        "multi" -> serverMultiThreaded(port)
        "limited" -> serverLimitedThreads(port)
        "pool" -> serverThreadPool(port)
        else -> println("unsupported mode '$mode'")
    }
}

fun main(args: Array<String>) {
    val argSizes = mapOf("server" to 3, "client" to 6, "servent" to 7)
    if (args.isEmpty() || argSizes[args[0]] != args.size) {
        println("""Usage:
            EchoUdp server <port> <single|multi|limited|pool>
            EchoUdp client <host> <port> <message> <iters> <delay_ms>
            EchoUdp servent <host> <client_port> <message> <delay_ms>
                <svr_port> <single|multi|limited|pool>
        """.trimIndent())
        exitProcess(1)
    }
    if (args[0] == "server") {
        server(args[1].toInt(), args[2])
        return
    }

    val skt = try {
        ObjSocket(Socket(args[1], args[2].toInt()))
    } catch (e: InterruptedException) {
        println(e)
        return
    } catch (e: IOException) {
        println(e)
        return
    }

    if (args[0] == "client") {
        client(skt, args[3], args[4].toInt(), args[5].toLong())
        skt.close()
        return
    }

    // servent
    val delay = args[4].toLong()
    val task = object : TimerTask() {
        override fun run() = client(skt, args[3], 1, 0)
    }
    Timer("ClientTimer").schedule(task, 0, delay)
    server(args[5].toInt(), args[6])
}