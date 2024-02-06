// This is the equivalent of ExecutorExample
import java.util.concurrent.Executors
import java.util.concurrent.TimeUnit
import kotlin.system.exitProcess

class MyCalculation(n: Int): Runnable {
    val values = Array(n){0}
    override fun run() {
        for (i in 0 until this.values.size) {
            values[i] = i * i
        }
    }
}

val THREADS = 10
val FAIL_CODE = 1
val pool = Executors.newFixedThreadPool(THREADS)

fun main(args: Array<String>) {
    if (args.size != 2 ) {
        println("Usage: ExecutorExample <n> <#tasks>")
        exitProcess(FAIL_CODE)
    }
    val n = args[0].toInt()
    val numTasks = args[1].toInt()
    val tasks = (1..numTasks).map { MyCalculation(n) }
    tasks.forEach(pool::execute)
    pool.shutdown()
    try {
        pool.awaitTermination(Long.MAX_VALUE, TimeUnit.SECONDS)
    } catch (e: InterruptedException) {
        println("interrupted while waiting for termination, tasks unfinished")
        e.printStackTrace()
        exitProcess(FAIL_CODE)
    }
    tasks.forEach { println(it.values.joinToString(" ")) }
}