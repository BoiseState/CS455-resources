all:

ProducerConsumer: ProducerConsumer.kt
	kotlinc ProducerConsumer.kt -include-runtime -d ProducerConsumer.jar

ThreadSafeAccount: ThreadSafeAccount.kt
	kotlinc ThreadSafeAccount.kt -include-runtime -d ThreadSafeAccount.jar

ThreadPool: ThreadPool.kt
	kotlinc ThreadPool.kt -include-runtime -d ThreadPool.jar

InterruptTest: InterruptTest.kt
	kotlinc InterruptTest.kt -include-runtime -d InterruptTest.jar

MaxThreads: MaxThreads.kt
	kotlinc MaxThreads.kt -include-runtime -d MaxThreads.jar

WrongPingPong: WrongPingPong.kt
	kotlinc WrongPingPong.kt -include-runtime -d WrongPingPong.jar

SyncPingPong: SyncPingPong.kt
	kotlinc SyncPingPong.kt -include-runtime -d SyncPingPong.jar

ThreadGroupExample: ThreadGroupExample.kt
	kotlinc ThreadGroupExample.kt -include-runtime -d ThreadGroupExample.jar

clean:
	rm *.jar
