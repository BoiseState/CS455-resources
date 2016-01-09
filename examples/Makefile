
all: zip onyx

onyx: clean
	rsync -av --delete sockets  onyx:cs555/lab/
	rsync -av --delete rmi-rpc  onyx:cs555/lab/
	rsync -av --delete serialization  onyx:cs555/lab/
	rsync -av --delete security  onyx:cs555/lab/
	rsync -av --delete threads  onyx:cs555/lab/
	rsync -av --delete Firewall-README  onyx:cs555/lab/
	rsync -av --delete multicasting  onyx:cs555/lab/
	rsync -av --delete sockets rmi-rpc security multicasting threads serialization doc Firewall-README ../www/lab/examples/

zip: clean
	zip -r cs555-lab.zip sockets rmi-rpc threads multicasting security doc serialization  Firewall-README -x \*.svn\* 
	mv cs555-lab.zip ../www/lab/

clean:
	cd sockets; make clean
	cd rmi-rpc; make clean
	cd security; make clean
	cd serialization; make clean
	cd multicasting; make clean
	cd threads; make clean


