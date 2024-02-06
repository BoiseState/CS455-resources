import java.awt.Color
import kotlin.reflect.KClass

fun main() {
    val type: Class<Color> = Color::class.java
    println("\n\n\n---------JAVA METHODS---------")
    // this will also show private methods
    type.declaredMethods.forEach { println(it) }
    println("\n\n\n---------JAVA FIELDS---------")
    type.declaredFields.forEach { println(it) }

    println("\n\n\n---------KOTLIN CLASS---------")
    // Kotlin Class
    val ktype: KClass<Color> = Color::class
    ktype.members.forEach { println(it) }
}