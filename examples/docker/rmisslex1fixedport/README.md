

Running this example on Docker
==============================

This example shows how to create a custom socket factory that uses a fixed port for all RMI communication 
over SSL. Note that rmiregistry is still on a separate port. Read the SSL section below to **make sure you
have setup the SSL part (below)** correctly.


Review Dockerfile and edit if necessary:

```
vim Dockerfile

sudo docker build -t dockerrmi-5111 .
sudo docker image ls
```

Make sure to use --net=host option for RMI to work! Or simply publish the rmiregistry port (5111)
with the -p 5111:5111 option to the dcker run command.

```
[amit@kohinoor docker(master)]$ sudo docker -p 5111:5111 dockerrmi-5111

DateServerImpl: Setting System Properties....
DateServerImpl: Created registry at port 5111
DateServerImpl: DateServerImpl bound in registry
===>Note the port and IP address printed out
```

In another console, run the client:
```
java rmisslex1fixedport.DateClient 172.17.0.2 5111
```

To stop the instance, find the instance name using the following command:

```
sudo docker ps

sudo docker stop <instance_name>
```

We can test the image by running it in an interactive mode as follows:

```
sudo docker run -it dockerrmi /bin/bash
```

Cleanups:

Remove all docker builds, caches, images etc

```
sudo docker system prune -a 
```

List all stopped (including stopped) containers.
```
sudo docker ps -a
```

Now remove specific containers:

```
sudo docker rm <containerid> [<containerid>...]
sudo docker rmi <imageid> [<imageid>...]
```


Running multiple instances on docker
====================================


For security reasons, rmiregistry only accepts bind from a server running on the same host
(in this case, a docker container).  So for multiple docker containers, each container needs
its own rmiregistry running and they need to be on different ports so clients can see them
all. 

The --net=host isn't strictly necessary as we can just publish the specific rmiregistry ports. The
one downside is that you would have to make multiple docker images with a different registry
port argument to the server in each -- but that is easy. Just modify the Dockerfile and generate a
new image -- it is super fast. 

In one terminal:

```
sudo docker run -p 5111:5111 dockerrmi-5111
```

In another terminal:

```
sudo docker run -p 5112:5112 dockerrmi-5112
```



Now the client can connect to either one:

```
java DateClient 172.17.0.2 5111

java DateClient 172.17.0.3 5112
```




Setting up SSL
===============

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
    the client.

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


