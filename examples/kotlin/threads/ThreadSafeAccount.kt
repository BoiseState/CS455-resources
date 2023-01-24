import kotlin.system.exitProcess
import java.util.concurrent.atomic.AtomicInteger

interface Account {
    fun deposit(amount: Int)
    fun balance(): Int
}

class UnsafeAccount: Account {
    private var _balance = 0
    override fun deposit(amount: Int) { this._balance += amount }
    override fun balance(): Int = this._balance
    override fun toString(): String = "unsafe balance = ${this._balance}"
}

class SyncAccount: Account{
    private var _balance = 0
    @Synchronized override fun deposit(amount: Int) { this._balance += amount }
    override fun balance(): Int = this._balance
    override fun toString(): String = "unsafe balance = ${this._balance}"
}

class AtomicAccount: Account {
    private var b = AtomicInteger()
    override fun deposit(amount: Int) { b.getAndAdd(amount) }
    override fun balance(): Int = b.get()
    override fun toString(): String = "balance = ${this.b.get()}"
}

class TestAccount(
    private val iters: Int,
    private val account: Account
): Runnable {
    override fun run() {
        for (i in 0 until iters) {
            this.account.deposit(1)
        }
    }
}

fun main(args: Array<String>) {
    if (args.size != 3 || args[2] !in setOf("sync", "unsafe", "atomic")) {
        println("Usage: TestAccount <numThreads> <iterations> <sync|unsafe|atomic>")
        exitProcess(1)
    }
    val numThreads = args[0].toInt()
    if (numThreads > 32) {
        println("max number of threads is 32")
    }
    val iters = args[1].toInt()
    val account = when(args[2]) {
        "sync" -> SyncAccount()
        "unsafe" -> UnsafeAccount()
        "atomic" -> AtomicAccount()
        // kotlin compiler isn't smart enough to figure out this is unreachable
        else -> return
    }
    val threads = 0.until(numThreads).map { Thread(TestAccount(iters, account)) }
    threads.forEach { it.start() }
    threads.forEach { it.join() }
    println(account)
}