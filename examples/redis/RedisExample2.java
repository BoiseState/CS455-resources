import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.UUID;

import redis.clients.jedis.RedisClient;

/**
 * A redis example where we store objects into a byte array to store into Redis and to revive from Redis. 
 * This is convenient but not necessarily desirable approach since we will have large byte arrays in Redis 
 * that are opaque so we cannot fully benefit from the capabilities of Redis.
 * 
 * @author
 */
public class RedisExample2
{

    private class Account {
	private String name;
	private UUID  id;
	private Date created;
	
	public Account(String name, UUID id, Date created) {
	    this.name = name;
	    this.id = id;
	    this.created = created;
	}
		
    }
    
    /**
     * @param args
     */
    public static void main(String[] args)
    {
	
		// choose a port in your assigned range for the project
		RedisClient pool = RedisClient.create("redis://localhost:6379");

		// Store & Retrieve a simple string
		pool.set("goo goo", "gaa gaa");
		System.out.println(pool.get("goo goo")); // prints "gaa gaa"
		
		pool.close();
    }

}
