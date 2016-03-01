import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;


/**
 * Caveat Emptor: Research the security of these methods before using them!
 *
 * MD2    The MD2 message digest algorithm as defined in  RFC 1319. 
 * MD5    The MD5 message digest algorithm as defined in  RFC 1321. 
 *
 * SHA-1      Hash algorithms defined in the  FIPS PUB 180-2.  
 * SHA-256    SHA-256 is a 256-bit hash function intended to provide 128
 * SHA-384    bits of security against collision attacks, while SHA-512 is
 * SHA-512    a 512-bit hash function intended to provide 256 bits of
 *            security. A 384-bit hash may be obtained by truncating the SHA-512 output. 
 */



public class SHA2Test
{
	/**
	 * Try encoding with MD5
	 * @param input
	 * @throws NoSuchAlgorithmException
	 */
	private static void tryMD5(String input)
	throws NoSuchAlgorithmException
	{
		System.out.println();
		MessageDigest md = MessageDigest.getInstance("MD5");
		System.out.println(md);
		byte[] bytes = input.getBytes();
		md.reset();

		byte[] result = md.digest(bytes);
		System.out.println();
		System.out.print("length (bits): "+result.length*8+" MD5sum: ");

		for (int i=0; i<result.length; i++) 
			System.out.printf("%X",result[i]);
		System.out.println();
	}

	/**
	 * Try encoding with SHA 512
	 * @param input
	 * @throws NoSuchAlgorithmException
	 */
	private static void trySHA(String input)
	throws NoSuchAlgorithmException
	{
		System.out.println();
		MessageDigest md = MessageDigest.getInstance("SHA-512");
		System.out.println(md);
		byte[] bytes = input.getBytes();
		md.reset();

		byte[] result = md.digest(bytes);
		System.out.println();
		System.out.print("length (bits): "+result.length*8+" SHA-512: ");

		for (int i=0; i<result.length; i++) 
			System.out.printf("%X",result[i]);
		System.out.println();
	}

	/**
	 * @param args
	 */
	public static void main (String args[])
	{
		if (args.length == 0) {
			System.err.println("Usage: java SHA2Test <string to encode>");
			System.exit(1);
		}
		
		try {
			tryMD5(args[0]);
			trySHA(args[0]);
		} catch (java.security.NoSuchAlgorithmException e) {
			System.err.println(e);
		}
		
	}
}
