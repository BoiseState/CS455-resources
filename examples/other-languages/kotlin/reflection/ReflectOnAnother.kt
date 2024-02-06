import java.lang.reflect.Modifier
import kotlin.reflect.KVisibility.*
import kotlin.reflect.full.declaredMembers
import kotlin.system.exitProcess

sealed class Animal {
    open fun makeNoise() {
        println("Make animal noise....")
    }
}
open class Cat : Animal() {
    override fun makeNoise() {
        println("Meow!")
    }
}
class Calico : Cat()

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
    println("JAVA PUBLIC METHODS")
    type.methods.forEach { println(it) }
    println("JAVA NON-PUBLIC METHODS")
    type.declaredMethods
        .filter { !Modifier.isPublic(it.modifiers) }
        .forEach { println(it) }
    println("\nJAVA PUBLIC FIELDS")
    type.fields.forEach { println(it) }
    println("\nJAVA NON-PUBLIC FIELDS")
    type.declaredFields
        .filter { !Modifier.isPublic(it.modifiers) }
        .forEach { println(it) }

    val ktype = type.kotlin
    println("\nPUBLIC KOTLIN MEMBERS")
    ktype.members.forEach { println(it) }

    println("\nNON-PUBLIC KOTLIN MEMBERS")
    ktype.declaredMembers
        .filter { it.visibility != PUBLIC }
        .forEach { println(it) }
}