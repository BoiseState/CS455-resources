import java.io.Serializable;
import java.util.Date;

/**
 * @author amit
 *
 */
public class MioAlmoDos implements Serializable {
	/**
	 * Generated serial UID for all versions of MioAlma objects
	 */
	private static final long serialVersionUID = -7208668478357756970L;

	private String name;
	private long uniqueId;
	private Date birthDate;
	private String email;
	private transient String interests;

	/**
	 * @param name
	 * @param uniqueId
	 */
	public MioAlmoDos(String name, long uniqueId) {
		this.name = name;
		this.uniqueId = uniqueId;
		this.birthDate = new Date(); // today
		this.interests = "daydreaming, space travel";
	}

	/**
	 * @param name
	 * @param uniqueId
	 * @param email
	 */
	public MioAlmoDos(String name, long uniqueId, String email) {
		this(name, uniqueId);
		this.email = email;
	}

	/**
	 * @return the email
	 */
	public String getEmail() {
		return email;
	}

	/**
	 * @param email
	 *            the email to set
	 */
	public void setEmail(String email) {
		this.email = email;
	}

	/**
	 * @return the name
	 */
	public String getName() {
		return name;
	}

	/**
	 * @param name
	 *            the name to set
	 */
	public void setName(String name) {
		this.name = name;
	}

	/**
	 * @return the uniqueId
	 */
	public long getUniqueId() {
		return uniqueId;
	}

	/**
	 * @return the birthDate
	 */
	public Date getBirthDate() {
		return birthDate;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString() {
		return "MioAlma [name=" + name + ", uniqueId=" + uniqueId + ", birthDate=" + birthDate + ", email=" + email
				+ ", interests=" + interests + "]";
	}

}
