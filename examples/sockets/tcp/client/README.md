
The tcp folder and  subfolders are packages, so run the programs from one level above this folder

cd ../

java tcp.client.DateAtHost localhost

This will only work if you jave installed and configured the xinetd daemon that provides the
daytime service at port 37. This is usually disbaled for security purposes.
