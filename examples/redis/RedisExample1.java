
import java.util.HashMap;
import java.util.List;
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
			account1.put("name", "Jane");
			account1.put("surname", "Ramirez");
			account1.put("company", "Redis");
			account1.put("age", "21");
			jedis.hset("user-account1", account1);
			System.out.println(jedis.hgetAll("user-account1"));
			// Prints: {name=Jane, surname=Ramirez, company=Redis, age=21}

			Map<String, String> account2 = new HashMap<String, String>();
			account2.put("name", "Neo");
			account2.put("surname", "Smith");
			account2.put("company", "Redis");
			account2.put("age", "21");
			jedis.hset("user-account2", account2);
			System.out.println(jedis.hgetAll("user-account2"));
			// Prints: {name=Neo, surname=Smith, company=Redis, age=21}

			
			// find all entries in a hash representing an account
			System.out.println();
			Map<String, String> value = jedis.hgetAll("user-account1");
			for (Entry<String, String> entry : value.entrySet()) {
				System.out.println(entry);
			}
			
			// find selected values from an account
			System.out.println();
			List<String> valueList = jedis.hmget("user-account1", "name", "company");
			System.out.println(valueList);
						
			// Redis does not let us directly store a HashMap with complex types as objects.
			// It is possible to serialize an object into a byte array and put it in as a value 
			// but then we lose some of the benefits of Redis so it isn't recommended.
			// To get around it, we could store a list of account names or a map of account names 
			// with values being whatever we want. Let us see how to use lists in Redis.

			
			System.out.println();
			int start = 1; int end = 0;
			//the following deletes the list if it exists because start > end
			jedis.ltrim("list", start, end); 
			
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