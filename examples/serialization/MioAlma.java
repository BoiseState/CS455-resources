import java.io.Serializable;
import java.util.Date;

/**
 * @author amit
 *
 */
public class MioAlma implements Serializable {
	/**
	 * Generated serial UID for all versions of MioAlma objects
	 */
	private static final long serialVersionUID = -7208668478357756970L;

	private String name;
	private long uniqueId;
	private Date birthDate;
	private transient String interests;

	/**
	 * @param name
	 * @param uniqueId
	 */
	public MioAlma(String name, long uniqueId) {
		this.name = name;
		this.uniqueId = uniqueId;
		this.birthDate = new Date(); // today
		this.interests = "daydreaming";
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
		return "MioAlma [name=" + name + ", uniqueId=" + uniqueId + ", birthDate=" + birthDate + ", interests="
				+ interests + "]";
	}

}
