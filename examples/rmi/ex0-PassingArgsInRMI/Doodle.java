import java.io.Serializable;

public class Doodle implements Serializable {
	private String dood = "Googoo-gaga";
	public void setDood(String dood) {
		this.dood = dood;
	}
	public String toString() {
		return "I am a: " + dood;
	}
}
