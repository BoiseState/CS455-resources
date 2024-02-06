import java.io.*
import java.net.ServerSocket
import java.net.Socket
import java.util.regex.Pattern
import kotlin.system.exitProcess


fun main(args: Array<String>) {
    if (args.size != 2) {
        println("Usage: TinyHttpd <port> <server_root>")
        exitProcess(1)
    }
    val skt = ServerSocket(args[0].toInt())
    val root = if (args[1].endsWith("/")) args[1] else args[1]+"/"
    println("Running TinyHttpd")
    var i = 0
    while (true) {
        val client = skt.accept()
        println("connected to client $i")
        serveClient(client, root)
        i += 1
    }
}

val GET = Pattern.compile("GET /?(\\S*).*")
fun serveClient(skt: Socket, root: String) {
    try {
        val input = BufferedReader(InputStreamReader(skt.getInputStream(), "8859_1"))
        val out: OutputStream = skt.getOutputStream()
        val pout = PrintWriter(OutputStreamWriter(out, "8859_1"), true)
        val req = input.readLine()
        if (req === null) {
            skt.close()
            return
        }
        println("TinyHttpd Received Request: $req");
        val match = GET.matcher(req)
        if (!match.matches()) {
            pout.println("400 Bad Request")
            return
        }
        val s = match.group(1)
        val filepath = when {
            s.endsWith("/") -> "$root$s/index.html"
            s.isEmpty() -> "${root}index.html"
            else -> root + s
        }
        try {
            pout.println("HTTP/1.1 200 OK")
            pout.println("TinyHttpd Server")
            pout.println("Content-Type: text/html")
            pout.println()
            println("/$filepath")
            val fis = FileInputStream(filepath)
            val data = ByteArray(64 * 1024)
            var read = fis.read(data)
            while (read > -1) {
                out.write(data, 0, read)
                read = fis.read(data)
            }
            out.flush()
            fis.close()
        } catch (e: FileNotFoundException) {
            pout.println("HTTP/1.1 404 Not Found");
        } catch (e: SecurityException) {
            println(e)
            pout.println("403 Forbidden");
        }
        skt.close()
    } catch (e: IOException) {
        println("IO Error: $e")
    }
}