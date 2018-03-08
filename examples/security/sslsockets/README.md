
#How to setup and run the example

Use the password test123 (as it is hardcoded in the Server.java and Client.java files for demo
purposes) Change to subfolder resources/ for Steps 1-6.

1. Create a keystore. (PKCS12 is an industry standard store type)

	keytool -deststoretype pkcs12 -genkey -alias SecureServer -keyalg RSA -keystore Server_Keystore

2. Check it.
	
	 keytool -list -v -keystore Server_Keystore   

3. Create a self-signed certificate

	keytool -export -alias SecureServer -keystore Server_Keystore -rfc -file Server.cer 

	or use the following to generate a certificate request to send to an authority

	keytool -export -alias SecureServer -keystore Server_Keystore -certreq 

4. Check the certificate

	cat Server.cer

5. Import the certificate into client's truststore

	keytool -import -alias Servercert -file Server.cer -keystore Client_Truststore

6. Examine contents of the truststore

	keytool -list -v -keystore Client_Truststore  

7. Start up the server:

	java Server 5005 &

8. Now run the client:

	java Client  localhost 5005

#Notes

In this example, we authenticated the server only. If client authentication is desired, you
will need to provide a similar keystore for the client's keys, and an appropriate truststore
for the server.

In our example, we are working with a self-signed certificate instead of certificates signed by
Certification Authority (CA). If there is a need to get the certificate signed by a CA then a
Certificate Signing Request(CSR) needs to be generated. The generated CSR, then, should to be
submitted along with other pertinent information to a Certification Authority such as VeriSign
or USPS, who will then digitally sign the certificate.


To see all the network transmissions, try

java -Djavax.net.debug=all Client localhost 5005


