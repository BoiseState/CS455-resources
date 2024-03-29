
#How to setup SSL

Change to subdirectory resources/ to do Steps 1-5. Use password test123 (as it is embedded in the code).

1. 	Generate a keystore that has a key pair (public and private key) along with a
	self-signed certificate. (PKCS12 is an industry standard store type)

	keytool -deststoretype pkcs12 -genkey -alias SecureServer -keyalg RSA -keystore Server_Keystore

2. 	Examine the contents of the generated Server Keystore

	keytool -list -v  -keystore Server_Keystore

3.  Create a self-signed certificate.

	keytool -export -alias SecureServer -keystore Server_Keystore -rfc -file Server.cer

4. 	To see what the certificate looks like.

	cat Server.cer

5.  Next we import the server certificate into a truststore that can be used by
    the client. Make sure to set a password on the client side (and use this in
	the client code as well!)

	keytool -import -alias SecureServer -file Server.cer -keystore Client_Truststore

6.	To verify the contents of the truststore that we created, we issue the following
    command. The contents of the truststore should contain a trustedCertEntry,
	which means that a private key is not available and should not be.

	keytool -list -v  -keystore Client_Truststore

In our example, we are working with a self-signed certificate instead of certificates signed by
Certification Authority (CA). If there is a need to get the certificate signed by a CA then a
Certificate Signing Request(CSR) needs to be generated. The generated CSR, then, should to be
submitted along with other pertinent information to a Certification Authority such as VeriSign
or USPS, who will then digitally sign the certificate.


#How to run the example

Now change back to the top-level security examples directory.

To run the server:

java rmisslex1.DateServerImpl &


To run the client:

java rmisslex1.DateClient localhost 


To see all the network transmissions, try

java -Djavax.net.debug=all rmisslex1.DateClient localhost


