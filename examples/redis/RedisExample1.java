
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;

/**
 * Simple example showing connection with Redis (REmote DIctionary Server).
 * 
 */

public class RedisExample1 {
	public static void main(String[] args)
	{
		// choose a port in your assigned range for the project
		JedisPool pool = new JedisPool("localhost", 6379);

		try (Jedis jedis = pool.getResource()) {
			// Store & Retrieve a simple string
			jedis.set("goo goo", "gaa gaa");
			System.out.println(jedis.get("goo goo")); // prints "gaa gaa"

			// Store & Retrieve a HashMap
			Map<String, String> account1 = new HashMap<String, String>();
			;
			account1.put("name", "Jane");
			account1.put("surname", "Ramirez");
			account1.put("company", "Redis");
			account1.put("age", "21");
			jedis.hset("user-account1", account1);
			System.out.println(jedis.hgetAll("user-account1"));
			// Prints: {name=Jane, surname=Ramirez, company=Redis, age=21}

			Map<String, String> account2 = new HashMap<String, String>();
			;
			account2.put("name", "Neo");
			account2.put("surname", "Smith");
			account2.put("company", "Redis");
			account2.put("age", "21");
			jedis.hset("user-account2", account2);
			System.out.println(jedis.hgetAll("user-account2"));
			// Prints: {name=Neo, surname=Smith, company=Redis, age=21}

			// It is possible to serialize an object into a byte array and put it in but
			// then we lose some of the benefits of Redis so it isn't recommended.

			System.out.println();
			Map<String, String> value = jedis.hgetAll("user-account1");
			for (Entry<String, String> entry : value.entrySet()) {
				System.out.println(entry);
			}

			System.out.println();
			jedis.ltrim("list", 1, 0); //this deletes the list if it already exists
			jedis.lpush("list", "user-account1", "user-account2");
			for (int i = 0; i < jedis.llen("list"); i++) {
				System.out.println(jedis.lindex("list", i));
			}
			System.out.println();
			// pushing the same elements again gives us duplicates as a list allows duplicates
			jedis.lpush("list", "user-account1", "user-account2");
			for (int i = 0; i < jedis.llen("list"); i++) {
				System.out.println(jedis.lindex("list", i));
			}
		}
		pool.close();
	}
}