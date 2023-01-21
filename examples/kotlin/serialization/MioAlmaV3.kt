import java.io.Serializable
import java.util.*

data class MioAlma(
    val name: String,
    val uniqueId: Long,
    val birthDate: Date,
    @Transient val interests: String,
): Serializable {
    companion object {
        // This is the same serial version UID as V1. V3 is now backwards
        // compatible with V1.
        private const val serialVersionUID = -3273057118377178019
    }
    val email = "$uniqueId@$name.com"

    override fun toString(): String {
        return "MioAlma [name=$name, uniqueId=$uniqueId, birthDate=$birthDate, email=$email, interests=$interests]"
    }
}