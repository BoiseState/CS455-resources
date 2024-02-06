import java.io.EOFException
import java.io.ObjectInputStream
import java.io.ObjectOutputStream
import java.io.Serializable
import java.lang.Exception
import java.net.ServerSocket
import java.net.Socket
import java.util.Date
import kotlin.system.exitProcess

class ObjStream(private val skt: Socket) {
    private val oout = ObjectOutputStream(skt.getOutputStream())
    private val oin = ObjectInputStream(skt.getInputStream())
    val remoteAddr = skt.remoteSocketAddress
    fun read(): Any? = try {
        oin.readObject()
    } catch (e: EOFException) {
        null
    }
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

data class DateRequest(val n: Int): Serializable
data class DateResponse(val n: Int, val date: Date): Serializable
data class WorkRequest(val n: Int): Serializable
data class WorkResponse(val array: Array<Int>): Serializable

fun server(port: Int) {
    val skt = ServerSocket(port)
    while(true) {
        val objStm = ObjStream(skt.accept())
        var req = objStm.read()
        while (req !== null) {
            val resp = response(req)
            objStm.write(resp)
            req = objStm.read()
        }
        objStm.close()
    }
}

fun response(req: Any): Any = when(req) {
    is DateRequest -> DateResponse(req.n, Date())
    is WorkRequest -> WorkResponse((1 until req.n).map { it * it }.toTypedArray())
    else -> throw Exception("Unsupported request $req")
}

fun client(host: String, port: Int, workFirst: Boolean) {
    val stm = ObjStream(Socket(host, port))
    val reqs = if (workFirst) {
        listOf(WorkRequest(6), DateRequest(0))
    } else {
        listOf(DateRequest(0), WorkRequest(6))
    }
    for (req in reqs) {
        stm.write(req)
        println(stm.read())
    }
}

fun main(args: Array<String>) {
    val argSizes = mapOf("server" to 2, "client" to 4)
    if (args.isEmpty() || argSizes[args[0]] != args.size) {
        println("""Usage:
            ObjectService server <port>
            ObjectService client <host> <port> <true|false>
        """.trimIndent())
        exitProcess(1)
    }
    when(args[0]) {
        "server" -> server(args[1].toInt())
        "client" -> client(args[1], args[2].toInt(), args[3].toBoolean())
    }
}