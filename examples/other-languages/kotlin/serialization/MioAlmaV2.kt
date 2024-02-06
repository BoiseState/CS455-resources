import java.io.Serializable
import java.util.*

data class MioAlma(
    val name: String,
    val uniqueId: Long,
    val birthDate: Date,
    @Transient val interests: String,
): Serializable {
    val email = "$uniqueId@$name.com"

    override fun toString(): String {
        return "MioAlma [name=$name, uniqueId=$uniqueId, birthDate=$birthDate, email=$email, interests=$interests]"
    }
}