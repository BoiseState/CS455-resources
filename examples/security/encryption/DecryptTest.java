

import java.io.*;
import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.BadPaddingException;
import java.security.Key;
import java.security.Security;
import java.security.NoSuchAlgorithmException;
import java.security.InvalidKeyException;

public class DecryptTest
{

    public static void main(String[] args) 
	{

        try {
			File dataFile = new File("data.encrypted");
			FileInputStream data = new FileInputStream(dataFile);
			FileInputStream fis = new FileInputStream("key");
			ObjectInputStream ois = new ObjectInputStream(fis);

            Key key = (Key) ois.readObject();
			ois.close();

            /*Cipher cipher = Cipher.getInstance("DES");*/
            Cipher cipher = Cipher.getInstance("AES");

			byte [] result = new byte[(int)dataFile.length()];
			int n = data.read(result);
			System.out.println("read "+n+" bytes");
			data.close();

            cipher.init(Cipher.DECRYPT_MODE, key);
            byte[] original = cipher.doFinal(result);
            System.out.println("Decrypted data: " + new String(original));

        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (NoSuchPaddingException e) {
            e.printStackTrace();
        } catch (InvalidKeyException e) {
            e.printStackTrace();
        } catch (IllegalStateException e) {
            e.printStackTrace();
        } catch (IllegalBlockSizeException e) {
            e.printStackTrace();
        } catch (BadPaddingException e) {
            e.printStackTrace();
        }

    }

}
