
#Running the server

In on terminal:

java -Djava.security.manager -Djava.security.policy=tinyhttpd/mysecurity.policy tinyhttpd.TinyHttpd port  

#Use a client to test

In another terminal:

Ask for the default index.html

java tinyhttpd.Client localhost 5005

or ask for specific url

java tinyhttpd.Client localhost 5005 sample.html

Or use a standard web browser and point it to

localhost:5005

or

localhost:5005/sample.html
