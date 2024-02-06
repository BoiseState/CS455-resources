// We can omit the string array if it's not used.
fun main() {
    // Example use case of Kotlin's special syntax. The Runnable interface has
    // a single method (void run()), so we can replace an instance of a class
    // with a lambda of type () -> Unit. Kotlin also allows us to omit the
    // parentheses when calling a function whose last argument is a lambda.
    val sleepyThread = Thread {
        while (true) {
            val name = Thread.currentThread().name
            try {
                println("$name: zzzz...")
                Thread.sleep(5000)
            } catch (e: InterruptedException) {
                println("$name: argh! let me sleep #\$@!")
            }
        }
    }
    sleepyThread.name = "SleepyThread"
    sleepyThread.start()
    // Now we have two threads, the main thread and the sleepy thread

    val name = Thread.currentThread().name
    Thread.sleep(5000)
    println("$name: disturbing the peacefully sleeping thread")
    sleepyThread.interrupt()

    Thread.sleep(5000)
    println("$name: wake up!")
    sleepyThread.interrupt()
}