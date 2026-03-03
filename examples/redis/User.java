import java.io.Serializable;

public class User implements Serializable {
    private static final long serialVersionUID = 3057360962217655744L;
    private String id;
    private String name;
    private int age;

    public User() {}  // Required for Jackson

    public User(String id, String name, int age) {
        this.id = id;
        this.name = name;
        this.age = age;
    }

    // Getters & Setters
    public String getId() { return id; }
    public void setId(String id) { this.id = id; }

    public String getName() { return name; }
    public void setName(String name) { this.name = name; }

    public int getAge() { return age; }
    public void setAge(int age) { this.age = age; }

    @Override
    public String toString() {
        return "User{id='" + id + "', name='" + name + "', age=" + age + "}";
    }
}