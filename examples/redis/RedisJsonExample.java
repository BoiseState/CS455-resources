import redis.clients.jedis.Jedis;
import tools.jackson.databind.*;

public class RedisJsonExample {

    public static void main(String[] args) throws Exception {

        Jedis jedis = new Jedis("localhost", 6379);
        tools.jackson.databind.ObjectMapper mapper = new ObjectMapper();

        User user = new User("1", "Alice", 30);

        // Serialize to JSON string
        String json = mapper.writeValueAsString(user);

        // Store in Redis
        jedis.set("user:1", json);

        // Retrieve from Redis
        String storedJson = jedis.get("user:1");

        // Deserialize back to object
        User retrievedUser = mapper.readValue(storedJson, User.class);

        System.out.println("Retrieved: " + retrievedUser);

        jedis.close();
    }
}