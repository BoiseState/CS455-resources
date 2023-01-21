import java.awt.Color
import java.io.FileInputStream
import java.io.FileNotFoundException
import java.io.FileOutputStream
import java.io.IOException
import java.io.ObjectInputStream
import java.io.ObjectOutputStream
import java.util.Hashtable
import kotlin.system.exitProcess

// Hashtable implements the serializable interface so it can be freeze-dried
fun save(filename: String) {
    println("saving hash table to file")
    val table = Hashtable<String, Any>()
    table["string"] = "Marco Polo"
    table["int"] = 1619
    table["double"] = Math.PI
    table["color"] = Color(255, 0, 0)

    try {
        val objSink = ObjectOutputStream(FileOutputStream(filename))
        objSink.writeObject(table)
        objSink.close()
    } catch (e: Exception) {
        println(e)
    }
}

fun load(filename: String) {
    val objSrc = ObjectInputStream(FileInputStream(filename))
    //Ok to suppress this warning as we know the type of serialized objects in the file
    //Another option is to cast to the base Hashtable type instead of the template
    //version. Basically what this is telling us is that the compiler has no way of knowing
    //what is contained in the external data file.
    @Suppress("UNCHECKED_CAST")
    val table = objSrc.readObject() as? Hashtable<String, Any>
    objSrc.close()
    if (table === null) {
        throw Exception("object could not be read as a hash table")
    }
    println(table.toString())
    for ((key, value) in table) {
        println("$key -> $value")
    }
}

fun main(args: Array<String>) {
    if (args.size != 2 || args[1] !in setOf("save", "load")) {
        println("Usage: SerHashTable filename <save|load>")
        exitProcess(1)
    }
    try {
        when(args[1]) {
            "save" -> save(args[0])
            "load" -> load(args[0])
        }
    } catch (e: ClassNotFoundException) {
        println(e)
    } catch (e: FileNotFoundException) {
        println(e)
    } catch (e: IOException) {
        println(e)
    }
}