import java.util.*
import kotlin.system.exitProcess

// Provides some functionality of a java.util.concurrent.BlockingQueue
class SharedQueue(private val capacity: Int) {
    private val queue = ArrayList<String>(capacity)
    private val lock = Object()

    @Synchronized fun put(item: String) {
        while (this.queue.size == this.capacity) {
            this.lock.wait()
        }
        queue.add(item)
        println("queue size is ${this.queue.size} after put")
        this.lock.notifyAll()
    }

    @Synchronized fun get(): String {
        this.lock.notifyAll()
        while (this.queue.size == 0) {
            this.lock.wait()
        }
        println("queue size is ${this.queue.size - 1} after get")
        return queue.removeAt(0)
    }
}

class Producer(
    private val q: SharedQueue,
    private val intervalMS: Long,
    private val id: Int,
): Thread() {
    override fun run() {
        try {
            while (true) {
                val s = "[${this.id}] ${Date()}"
                this.q.put(s)
                println("Producer ${this.id} pushed message '$s'")
                sleep(this.intervalMS)
            }
        } catch (e: InterruptedException) {
            println("Producer ${this.id} terminated")
        }
    }
}

class Consumer(
    private val q: SharedQueue,
    private val intervalMS: Long,
    private val id: Int,
): Thread() {
    override fun run() {
        try {
            while (true) {
                val s = this.q.get()
                println("Consumer ${this.id} pulled message '$s'")
                sleep(this.intervalMS)
            }
        } catch (e: InterruptedException) {
            println("Consumer ${this.id} terminated")
        }
    }
}

fun main(args: Array<String>) {
    if (args.size != 5) {
        println(arrayOf(
            "Usage:  PC",
            "<queue size>",
            "<#producers>",
            "<#consumers>",
            "<producer_sleep_time(millisecs)>",
            "<consumer_sleep_time(millisecs)>",
        ).joinToString(" "))
        exitProcess(1)
    }

    val capacity = args[0].toInt()
    val prodNum = args[1].toInt()
    val consNum = args[2].toInt()
    val prodDelay = args[3].toLong()
    val consDelay = args[4].toLong()

    val q = SharedQueue(capacity)

    val producers = 0.until(prodNum).map { Producer(q, prodDelay, it) }
    val consumers = 0.until(consNum).map { Consumer(q, consDelay, it) }
    // Uses Kotlin's lambda shorthand for single abstract method (SAM)
    // instances. Thread constructor takes a Runnable object. Because Runnable
    // only has one method of type () -> Unit, you can replace an instance of
    // Runnable with a lambda.
    Runtime.getRuntime().addShutdownHook(Thread {
        producers.forEach { prod ->
            prod.interrupt()
            prod.join()
        }
        consumers.forEach { cons ->
            cons.interrupt()
            cons.join()
        }
    })
    producers.forEach { it.start() }
    // function reference syntax
    consumers.forEach(Consumer::start)
}
