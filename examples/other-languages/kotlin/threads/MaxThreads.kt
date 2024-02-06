import kotlin.concurrent.thread

const val MAX_THREADS: Int = 50_000

// Got this error once while trying to Ctrl-C out of it:
// OpenJDK 64-Bit Server VM warning: Exception java.lang.OutOfMemoryError
// occurred dispatching signal SIGINT to handler- the VM may need to be forcibly
// terminated
fun main() {
    for (i in 0..MAX_THREADS) {
        thread {
            println("Running thread number $i")
            try {
                Thread.sleep(20_000)
            } catch (e: InterruptedException) {
                e.printStackTrace()
            }
        }
    }
}