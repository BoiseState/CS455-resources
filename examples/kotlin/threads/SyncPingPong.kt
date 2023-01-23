import kotlin.system.exitProcess

fun main(args: Array<String>) {
    if (args.size != 1) {
        println("Usage: java SynchronizedPingPong <delay(ms)>")
        exitProcess(1)
    }
    val delay = args[0].toLong()
    val ping = Ping(delay)
    val pong = Pong(delay)
    ping.pong = pong
    pong.ping = ping
    Thread(pong).start()
    Thread(ping).start()
}

class Ping(private val delay: Long): Runnable {
    val cond = Object()
    var pong: Pong? = null

    override fun run() {
        val pong = this.pong ?: throw IllegalStateException("pong must be initialized")
        try {
            while(true) {
                println("ping")
                Thread.sleep(this.delay)
                synchronized(pong.cond) {
                    pong.cond.notify()
                }
                synchronized(this.cond) {
                    this.cond.wait()
                }
            }
        } catch(e: InterruptedException) {
            println(e)
        }
    }
}

class Pong(private val delay: Long): Runnable {
    val cond = Object()
    var ping: Ping? = null

    override fun run() {
        val ping = this.ping ?: throw IllegalStateException("ping must be initialized")
        try {
            while(true) {
                synchronized(this.cond) {
                    this.cond.wait()
                }
                println("PONG")
                Thread.sleep(this.delay)
                synchronized(ping.cond) {
                    ping.cond.notify()
                }
            }
        } catch(e: InterruptedException) {
            println(e)
        }
    }
}