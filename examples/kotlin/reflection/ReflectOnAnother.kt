import kotlin.reflect.KVisibility.*
import kotlin.reflect.full.declaredMembers
import kotlin.system.exitProcess

fun main(args: Array<String>) {
    if (args.size != 1) {
        println("Usage: ReflectOnAnother <class_name>")
        exitProcess(1)
    }
    val type = try {
        Class.forName(args[0])
    } catch (e: ClassNotFoundException) {
        println("class '${args[0]}' does not exist")
        exitProcess(1)
    }
    println("JAVA METHODS")
    type.declaredMethods.forEach { println(it) }
    println("\nJAVA FIELDS")
    type.declaredFields.forEach { println(it) }

    val ktype = type.kotlin
    println("\nPUBLIC KOTLIN MEMBERS")
    ktype.declaredMembers
        .filter { it.visibility == PUBLIC }
        .forEach { println(it) }

    println("\nNON-PUBLIC KOTLIN MEMBERS")
    ktype.declaredMembers
        .filter { it.visibility != PUBLIC }
        .forEach { println(it) }
}