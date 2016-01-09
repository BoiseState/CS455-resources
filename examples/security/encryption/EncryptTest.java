

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

public class EncryptTest
{

    public static void main(String[] args) 
	{
        try {
			FileOutputStream fos = new FileOutputStream("key");
			FileOutputStream dataFile = new FileOutputStream("data.encrypted");
			ObjectOutputStream oos = new ObjectOutputStream(fos);

            KeyGenerator kg = KeyGenerator.getInstance("AES");
            /*KeyGenerator kg = KeyGenerator.getInstance("DES");*/
			System.out.println(kg.getAlgorithm() + " " + kg.getProvider());
            Key key = kg.generateKey();

            System.out.println(key);
			oos.writeObject(key);
			oos.close();
			File keyFile = new File("key");
			keyFile.setReadable(false, false);
			keyFile.setWritable(false, false);
			keyFile.setReadable(true, true);

            Cipher cipher = Cipher.getInstance("AES");
            /*Cipher cipher = Cipher.getInstance("DES");*/
            byte[] data = "Hello World!".getBytes();
            System.out.println("Original data : " + new String(data));

            cipher.init(Cipher.ENCRYPT_MODE, key);
            byte[] result = cipher.doFinal(data);
            System.out.println("Encrypted data: " + new String(result));
            System.out.println("Encrypted data saved in file named: data.encrypted");
			dataFile.write(result);
			dataFile.close();
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
