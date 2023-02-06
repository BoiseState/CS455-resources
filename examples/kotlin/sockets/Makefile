all:

EchoUdp: EchoUdp.kt
	kotlinc EchoUdp.kt -include-runtime -d EchoUdp.jar

TcpEcho: TcpEcho.kt
	kotlinc TcpEcho.kt -include-runtime -d TcpEcho.jar

ObjectService: ObjectService.kt
	kotlinc ObjectService.kt -include-runtime -d ObjectService.jar

TinyHttpd: TinyHttpd.kt
	kotlinc TinyHttpd.kt -include-runtime -d TinyHttpd.jar

clean:
	rm *.jar
