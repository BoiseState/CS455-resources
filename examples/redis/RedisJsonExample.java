import com.fasterxml.jackson.core.JacksonException;
import com.fasterxml.jackson.databind.ObjectMapper;

import redis.clients.jedis.Jedis;


public class RedisJsonExample {

    public static void main(String[] args) throws JacksonException {

        Jedis jedis = new Jedis("localhost", 6379);
        ObjectMapper mapper = new ObjectMapper();
        
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