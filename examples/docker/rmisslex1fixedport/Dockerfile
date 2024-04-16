FROM fedora
RUN  dnf -y install net-tools git java java-devel make
WORKDIR /home
RUN  (cd /home; git clone https://github.com/BoiseState/CS455-resources.git)
RUN (cd /home/CS455-resources/examples/docker/rmisslex1fixedport; javac *.java)
RUN /usr/sbin/ifconfig
CMD (cd /home/CS455-resources/examples/docker/rmisslex1fixedport; java DateServerImpl 5111)
