import io.grpc.Status
import io.grpc.StatusRuntimeException
import io.grpc.netty.shaded.io.grpc.netty.GrpcSslContexts
import io.grpc.netty.shaded.io.grpc.netty.NettyChannelBuilder
import kotlinx.coroutines.DelicateCoroutinesApi
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.receiveAsFlow
import kotlinx.coroutines.launch
import io.grpc.netty.shaded.io.grpc.netty.NettyServerBuilder
import io.grpc.netty.shaded.io.netty.handler.ssl.ClientAuth
import io.grpc.netty.shaded.io.netty.handler.ssl.SslProvider
import io.grpc.protobuf.services.ProtoReflectionService
import servicegen.*
import java.io.File
import java.lang.Exception
import kotlin.system.exitProcess
import servicegen.StreamingServiceExampleGrpcKt.StreamingServiceExampleCoroutineStub

object ENV {
    val CERT = File(System.getenv("STREAM_SERVER_CERT"))
    val PRIV = File(System.getenv("STREAM_SERVER_PRIVATE_KEY") ?: "")
}

class StreamingExampleServer(
    port: Int,
    enableSsl: Boolean
): StreamingServiceExampleGrpcKt.StreamingServiceExampleCoroutineImplBase() {
    val svr = if (enableSsl) {
        NettyServerBuilder.forPort(port)
            .sslContext(GrpcSslContexts
                .forServer(ENV.CERT, ENV.PRIV, null)
                .sslProvider(SslProvider.OPENSSL)
                .clientAuth(ClientAuth.NONE)
                .build()
            )
            .addService(this)
            .addService(ProtoReflectionService.newInstance())
            .build()
    } else {
        NettyServerBuilder
            .forPort(port)
            .addService(this)
            .addService(ProtoReflectionService.newInstance())
            .build()
    }

    fun start() {
        println("running grpc server with reflection")
        svr.start()
        svr.awaitTermination()
    }

    override suspend fun unaryCall(request: UnaryRequest): UnaryResponse {
        if (request.errorOut) {
            val msg = "you caused a unary error!"
            val status = Status.INVALID_ARGUMENT.withDescription(msg)
            throw StatusRuntimeException(status)
        } else {
            return unaryResponse { msg = "you made a valid request!" }
        }
    }

    @OptIn(DelicateCoroutinesApi::class)
    // A "flow" is a lazily evaluated sources of objects, that you consume on the client end.
    override fun grabSeries(request: GrabSeriesRequest): Flow<SeriesEntry> {
        if (request.errorOut) {
            val msg = "you caused a streaming error!"
            val status = Status.INVALID_ARGUMENT.withDescription(msg)
            throw StatusRuntimeException(status)
        }
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

class StreamingExampleClient(host: String, port: Int, enableSsl: Boolean) {
    private val client = if (enableSsl)  {
        StreamingServiceExampleCoroutineStub(NettyChannelBuilder
            .forAddress(host, port)
            .sslContext(GrpcSslContexts
                .forClient()
                .trustManager(ENV.CERT)
                .sslProvider(SslProvider.OPENSSL)
                .build()
            )
            .build()
        )
    } else {
        StreamingServiceExampleCoroutineStub(
            NettyChannelBuilder.forAddress(host, port).usePlaintext().build()
        )
    }

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
    println("""Usage: 
        StreamingExample server <enable_ssl> <port>
        StreamingExample client <enable_ssl> <host> <port> unary <error_out>
        StreamingExample client <enable_ssl> <host> <port> stream <error_out> <delay_ms> <amount>
        """.trimIndent())
    exitProcess(1)
}

suspend fun main(args: Array<String>) {
    printUsage(args.size in setOf(3, 6, 8))
    if (args.size == 3) {
        printUsage(args[0] == "server")
        StreamingExampleServer(args[2].toInt(), args[1].toBooleanStrict()).start()
        return
    }
    printUsage(args[0] == "client")
    val client = StreamingExampleClient(args[2], args[3].toInt(), args[1].toBooleanStrict())
    if (args.size == 6) {
        printUsage(args[4] == "unary")
        client.unary(args[5].toBooleanStrict())
        return
    }
    if (args.size == 8) {
        printUsage(args[4] == "stream")
        client.stream(args[5].toBooleanStrict(), args[6].toLong(), args[7].toInt())
        return
    }
}