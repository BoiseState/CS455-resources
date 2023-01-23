import java.io.FileInputStream
import java.io.FileOutputStream
import java.io.IOException
import java.io.ObjectInputStream
import java.io.ObjectOutputStream
import java.lang.Exception
import java.util.Date
import kotlin.system.exitProcess

fun freeze(file: String) {
    val obj = MioAlma("hello", 1234, Date(), "food, soccer")
    val objSink = ObjectOutputStream(FileOutputStream(file))
    objSink.writeObject(obj)
    objSink.close()
}

fun revive(file: String) {
    try {
        val objSrc = ObjectInputStream(FileInputStream(file))
        val obj = objSrc.readObject()
        if (obj !is MioAlma) {
            println("bad object $obj")
            return
        }
        println("Deserialized object: $obj")
    } catch (e: IOException) {
        println(e)
    } catch (e: ClassNotFoundException) {
        println(e)
    }
}

fun main(args: Array<String>) {
    if (args.size != 2 || args[1] !in setOf("freeze", "revive")) {
        println("Usage: Cryogenics filename <freeze|revive>")
        exitProcess(1)
    }
    when(args[1]) {
        "freeze" -> freeze(args[0])
        "revive" -> revive(args[0])
    }
}
