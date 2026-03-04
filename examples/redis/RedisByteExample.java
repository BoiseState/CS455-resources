import redis.clients.jedis.RedisClient;

public class RedisByteExample {

    public static void main(String[] args) throws Exception {

        RedisClient jedis = RedisClient.create("redis://localhost:6379");

        User user = new User("2", "Bob", 25);

        // Serialize to byte array
        byte[] bytes = Utility.getBytes(user);

        // Store in Redis
        jedis.set("user:2".getBytes(), bytes);

        // Retrieve from Redis
        byte[] retrievedBytes = jedis.get("user:2".getBytes());

        // Deserialize back to object
        User retrievedUser = (User) Utility.getObject(retrievedBytes);
        System.out.println("Retrieved: " + retrievedUser);
        jedis.close();
    }
}