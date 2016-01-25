import java.io.Serializable;
import java.util.Date;

/**
 * @author amit
 *
 */
public class MioAlmoDos implements Serializable
{
    /**
     * Generated serial UID for all versions of MioAlma objects
     */
    private static final long serialVersionUID = -7208668478357756970L;
    
    private String name;
    private long uniqueId;
    private Date birthDate;
    private String gender;
    private transient String interests;

    /**
     * @param name
     * @param uniqueId
     */
    public MioAlmoDos(String name, long uniqueId)
    {
	this.name = name;
	this.uniqueId = uniqueId;
	this.birthDate = new Date(); //today
	this.interests = "daydreaming, space travel";
    }
    
    /**
     * @param name
     * @param uniqueId
     * @param gender
     */
    public MioAlmoDos(String name, long uniqueId, String gender)
    {	
	this(name, uniqueId);
	this.gender = gender;
    }

    /**
     * @return the gender
     */
    public String getGender()
    {
        return gender;
    }

    /**
     * @param gender the gender to set
     */
    public void setGender(String gender)
    {
        this.gender = gender;
    }

    

    /**
     * @return the name
     */
    public String getName()
    {
	return name;
    }

    /**
     * @param name
     *            the name to set
     */
    public void setName(String name)
    {
	this.name = name;
    }

    /**
     * @return the uniqueId
     */
    public long getUniqueId()
    {
	return uniqueId;
    }

    /**
     * @return the birthDate
     */
    public Date getBirthDate()
    {
	return birthDate;
    }

    /* (non-Javadoc)
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString()
    {
	return "MioAlma [name=" + name + ", uniqueId=" + uniqueId + ", birthDate=" + birthDate + ", gender=" + gender +  ", interests="
	        + interests + "]";
    }

}
