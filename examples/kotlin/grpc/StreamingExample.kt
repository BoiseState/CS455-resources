import io.grpc.ManagedChannelBuilder
import io.grpc.ServerBuilder
import io.grpc.Status
import io.grpc.StatusRuntimeException
import kotlinx.coroutines.DelicateCoroutinesApi
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.receiveAsFlow
import kotlinx.coroutines.launch
import servicegen.*
import java.lang.Exception
import kotlin.system.exitProcess

class StreamingExampleServer(
    port: Int
): StreamingServiceExampleGrpcKt.StreamingServiceExampleCoroutineImplBase() {
    val svr = ServerBuilder.forPort(port).addService(this).build()

    fun start() {
        println("running grpc server")
        svr.start()
        svr.awaitTermination()
    }

    override suspend fun unaryCall(request: UnaryRequest): UnaryResponse {
        if (request.errorOut) {
            val msg = "you've caused an error!"
            val status = Status.INVALID_ARGUMENT.withDescription(msg)
            throw StatusRuntimeException(status)
        } else {
            return unaryResponse { msg = "you made a valid request!" }
        }
    }

    @OptIn(DelicateCoroutinesApi::class)
    // A "flow" is a lazily evaluated sources of objects, that you consume on the client end.
    override fun grabSeries(request: GrabSeriesRequest): Flow<SeriesEntry> {
        // A channel is a thread-safe pipe for passing from senders to receivers on threads
        val chan = Channel<SeriesEntry>(20)
        // effectively a daemon thread, this doesn't belong to any coroutine
        // scope. It won't be automatically be joined with any other coroutine,
        // it will only terminate when the function naturally ends or an
        // exception is thrown.
        GlobalScope.launch {
            try {
                for (i in 0 until request.amount) {
                    delay(request.delayMS)
                    chan.send(seriesEntry { entry = "message #$i" })
                }
                chan.close()
            } catch (e: Exception) {
                println(e)
            }
        }
        return chan.receiveAsFlow()
    }
}

class StreamingExampleClient(host: String, port: Int) {
    private val client = StreamingServiceExampleGrpcKt.StreamingServiceExampleCoroutineStub(
        ManagedChannelBuilder.forAddress(host, port).usePlaintext().build()
    )

    suspend fun unary(error: Boolean) {
        try {
            println("making unary call")
            val resp = client.unaryCall(unaryRequest { errorOut = error })
            println("unary response $resp")
        } catch (e: Exception) {
            println(e)
        }
    }

    suspend fun stream(error: Boolean, delayMillis: Long, number: Int) {
        val resp = try {
            println("making streaming call")
            client.grabSeries( grabSeriesRequest{
                errorOut = error
                delayMS = delayMillis
                amount = number
            })
        } catch (e: Exception) {
            println("exception during request: $e")
            return
        }
        try {
            resp.collect {
                println("got stream response: $it")
            }
            println("stream ended")
        } catch (e: Exception) {
            println("exception during streaming: $e")
            return
        }
    }
}

fun printUsage(stay: Boolean = false) {
    if (stay) {
        return
    }
    println("""
        Usage: 
        StreamingExample server <port>
        StreamingExample client <host> <port> unary <error_out>
        StreamingExample client <host> <port> stream <error_out> <delay_ms> <amount>
        """.trimIndent()
    )
    exitProcess(1)
}

suspend fun main(args: Array<String>) {
    val printUsage = { stay: Boolean, point: String ->
        if (!stay) {
            println(point)
            println(args.joinToString(" ") { "\"$it\"" })
            println("""
                Usage: 
                StreamingExample server <port>
                StreamingExample client <host> <port> unary <error_out>
                StreamingExample client <host> <port> stream <error_out> <delay_ms> <amount>
                """.trimIndent()
            )
            exitProcess(1)
        }
    }
    printUsage(args.size in setOf(2, 5, 7), "length check")
    if (args.size == 2) {
        printUsage(args[0] == "server", "server arg")
        StreamingExampleServer(args[1].toInt()).start()
        return
    }
    printUsage(args[0] == "client", "client arg")
    val client = StreamingExampleClient(args[1], args[2].toInt())
    if (args.size == 5) {
        printUsage(args[3] == "unary", "unary arg")
        client.unary(args[4].toBoolean())
        return
    }
    if (args.size == 7) {
        printUsage(args[3] == "stream", "stream arg")
        client.stream(args[4].toBoolean(), args[5].toLong(), args[6].toInt())
        return
    }
}