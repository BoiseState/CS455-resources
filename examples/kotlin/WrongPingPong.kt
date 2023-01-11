import kotlin.concurrent.thread
import kotlin.system.exitProcess

// Fun with first-class functions. This function return a closure (which is a
// kind of function) to be run as a thread.
fun player(word: String, delay: Long): () -> Unit = {
    try {
        while(true) {
            println(word)
            Thread.sleep(delay)
        }
    } catch (_: InterruptedException) {}
}

fun main(args: Array<String>) {
    if (args.size != 1) {
        println("Usage: java PingPong <delay(ms)>")
        exitProcess(1)
    }
    val delay = args[0].toLong()
    thread(block = player("ping", delay))
    thread(block = player("PONG", delay))
}