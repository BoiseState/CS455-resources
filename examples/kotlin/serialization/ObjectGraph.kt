import java.io.FileInputStream
import java.io.FileOutputStream
import java.io.ObjectInputStream
import java.io.ObjectOutputStream
import java.io.Serializable

data class Dummy(var value: Int): Serializable {
    override fun toString(): String = "[value=$value]"
}

data class Twins(val d1: Dummy, val d2: Dummy): Serializable {
    override fun toString(): String = "[d1=$d1, d2=$d2]"
}

fun main() {
    val d = Dummy(3)
    val dos = Twins(d, d)
    println("initial value")
    println(dos)
    println("changing")
    dos.d1.value = 4
    println(dos)
    println("serializing")
    val objOut = ObjectOutputStream(FileOutputStream("object_graph.ser"))
    objOut.writeObject(dos)
    objOut.close()
    println("deserializing")
    val objIn = ObjectInputStream(FileInputStream("object_graph.ser"))
    val deser = objIn.readObject() as Twins
    println(deser)
    // The object graph is preserved after deserialization, both references
    // see the update
    println("changing deserialized value")
    dos.d1.value = 5
    println(dos)
}