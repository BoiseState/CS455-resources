import java.io.Serializable
import java.util.Date

// V3 has the same serial version UID, you can deserialize a V3 object with V1.
// It's just going to ignore the new 'email' field in V3.
data class MioAlma(
    val name: String,
    val uniqueId: Long,
    val birthDate: Date,
    // Deserialization can break Kotlin's null safety. Since the deserializer is
    // told to ignore this field, it's never set and remains at the default
    // value for String in Java, which is null, even though Kotlin's type system
    // doesn't allow this.
    @Transient val interests: String,
): Serializable {
    override fun toString(): String {
        return "MioAlma [name=$name, uniqueId=$uniqueId, birthDate=$birthDate, interests=$interests]"
    }
}