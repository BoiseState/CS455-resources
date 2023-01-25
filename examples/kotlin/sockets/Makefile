all:

EchoUdp: EchoUdp.kt
	kotlinc EchoUdp.kt -include-runtime -d EchoUdp.jar

TcpEcho: TcpEcho.kt
	kotlinc TcpEcho.kt -include-runtime -d TcpEcho.jar

ObjectService: ObjectService.kt
	kotlinc ObjectService.kt -include-runtime -d ObjectService.jar

clean:
	rm *.jar
