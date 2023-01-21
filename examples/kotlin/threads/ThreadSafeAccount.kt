import kotlin.system.exitProcess

// You need to make a class 'open' to extend it. Kotlin classes are final by
// default
open class Account {
    // protected means something different here than in Java. It makes a field
    // accessible only to a class and its subclasses, not the entire module. If
    // you want the equivalent of Java's protected modifier, use the internal
    // modifier in Kotlin instead.
    var balance = 0.0

    open fun deposit(amount: Double) {
        this.balance += amount
    }

    open fun withdraw(amount: Double) {
        this.balance -= amount
    }

    override fun toString(): String {
        return "balance = ${this.balance}"
    }
}

class SyncAccount: Account() {
    @Synchronized override fun deposit(amount: Double) {
        // This is just to illustrate the protected modifier. We'd probably
        // want to use the same pattern withdraw uses otherwise.
        this.balance += amount
    }

    @Synchronized override fun withdraw(amount: Double) {
        super.withdraw(amount)
    }
}

class TestAccount(
    private val iters: Int,
//    private val account: Account
): Thread() {
    val account = Account()
    override fun run() {
        for (i in 0 until iters) {
            this.account.deposit(1.0)
        }
    }
}

fun main(args: Array<String>) {
    if (args.size != 3) {
        println("Usage: TestAccount <numThreads> <iterations> <good|bad>")
        exitProcess(1)
    }
    val numThreads = args[0].toInt()
    if (numThreads > 32) {
        println("max number of threads is 32")
    }
    val iters = args[1].toInt()
    val threads = 0.until(numThreads).map { TestAccount(iters) }
    threads.forEach { it.start() }
    threads.forEach { it.join() }
    val total = threads.sumOf { it.account.balance }
    println("total amount: $total")
}