fun runThread() {
    while(true) {
        try {
            Thread.sleep(10_000)
        } catch(e: InterruptedException) {
            println(e)
        }
    }
}

fun main() {
    val group = ThreadGroup("TaskGroup")

    val t1 = Thread(group, {runThread()}, "one")
    val t2 = Thread(group, {runThread()}, "two")
    val t3 = Thread(group, {runThread()}, "three")

    t1.start()
    t2.start()
    t3.start()

    println("thread group has ${group.activeCount()} active threads")
    val threads = Array<Thread?>(3){null}
    group.enumerate(threads)
    threads.forEach { println(it!!.toString()) }
    println(group.toString())
    group.stop() // halts execution of all threads in the group
    // no more threads should be running, so the JVM will quit
}