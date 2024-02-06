import kotlin.system.exitProcess

fun main(args: Array<String>) {
    if (args.size != 1) {
        println("Usage: TraceAncestry <class_name>")
        exitProcess(1)
    }
    var type = try {
        Class.forName(args[0])
    } catch (e: ClassNotFoundException) {
        println("class '${args[0]}' does not exist")
        exitProcess(1)
    }
    while (type !== null) {
        println(type.canonicalName)
        type = type.superclass
    }
}