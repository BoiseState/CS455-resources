import java.io.Serializable;
import java.util.Date;

/**
 * @author amit
 *
 */
public class Mio implements Serializable
{
    private String name;
    private long uniqueId;
    private Date birthDate;

    /**
     * @param name
     * @param uniqueId
     */
    public Mio(String name, long uniqueId)
    {
	this.name = name;
	this.uniqueId = uniqueId;
	this.birthDate = new Date(); //today
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
	return "Mio [" + (name != null ? "name=" + name + ", " : "") + "uniqueId=" + uniqueId + ", "
	        + (birthDate != null ? "birthDate=" + birthDate : "") + "]";
    }

}
