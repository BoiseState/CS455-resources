all: TinyHttpd.kt Dockerfile http_root/ mysecurity.policy
	kotlinc TinyHttpd.kt -include-runtime -d TinyHttpd.jar
	docker build -f Dockerfile -t tinyhttpd:latest .	

clean:
	rm *.jar
