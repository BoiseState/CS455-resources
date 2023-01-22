import java.lang.IllegalArgumentException
import java.lang.reflect.InvocationTargetException
import kotlin.reflect.full.createInstance
import kotlin.system.exitProcess

class Dummy() {
    companion object {
        @JvmStatic fun associatedHello(s: String): String = "we're associated, $s!"
    }
    fun hello(s: String): String = "hello, $s!"
    private fun goodbye(s: String): String = "goodbye, $s!"
}

fun main(args: Array<String>) {
    if (args.size != 4 || args[1] !in setOf("static", "instance")) {
        println("Usage: InvokeMethod <class_name> <static|instance> <method_name> <string_argument>")
        exitProcess(1)
    }
    try {
        val kclass = Class.forName(args[0]).kotlin
        val instance = if (args[1] == "static") null else {
            kclass.createInstance()
        }
        val method = kclass.java.getDeclaredMethod(args[2], String::class.java)
        // you can even access private methods this way
        method.isAccessible = true
        println(method.invoke(instance, args[3]))
    } catch (e: IllegalAccessException) {
        println("not allowed to call this method: $e")
    } catch (e: ClassNotFoundException) {
        println("could not find class: ${args[0]}")
    } catch (e: NoSuchMethodException) {
        println("could not find method ${args[2]} in class ${args[0]}")
    } catch(e: IllegalArgumentException) {
        println("bad parameters passed to the method")
    } catch(e: InvocationTargetException) {
        println("exception occurred invoking method: ${e.targetException}")
    }
}