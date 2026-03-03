import redis.clients.jedis.RedisClient;

import java.io.*;

public class RedisByteExample {

    public static void main(String[] args) throws Exception {

        RedisClient jedis = RedisClient.create("redis://localhost:6379");

        User user = new User("2", "Bob", 25);

        // Serialize to byte array
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        ObjectOutputStream out = new ObjectOutputStream(bos);
        out.writeObject(user);
        out.flush();
        byte[] bytes = bos.toByteArray();

        // Store in Redis
        jedis.set("user:2".getBytes(), bytes);

        // Retrieve from Redis
        byte[] retrievedBytes = jedis.get("user:2".getBytes());

        // Deserialize back to object
        ByteArrayInputStream bis = new ByteArrayInputStream(retrievedBytes);
        ObjectInputStream in = new ObjectInputStream(bis);
        User retrievedUser = (User) in.readObject();

        System.out.println("Retrieved: " + retrievedUser);

        jedis.close();
    }
}