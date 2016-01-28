
all: subdirs 

subdirs:
	cd client/; make
	cd objectserver/; make
	cd socketoptions/; make
	cd singlethreaded/; make
	cd multithreaded/; make

clean:
	cd client/; make clean
	cd objectserver/; make clean
	cd socketoptions/; make clean
	cd singlethreaded/; make clean
	cd multithreaded/; make clean
	/bin/rm -f *.class
