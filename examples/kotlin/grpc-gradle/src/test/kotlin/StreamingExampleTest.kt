import io.grpc.ManagedChannelBuilder
import io.grpc.Status.Code.INVALID_ARGUMENT
import io.grpc.StatusRuntimeException
import org.junit.jupiter.api.AfterAll
import org.junit.jupiter.api.BeforeAll
import kotlin.test.*

class IdServerTest {
    companion object {
        @JvmStatic val svr = StreamingExampleServer(50051, false)
        @JvmStatic @BeforeAll fun startServer() {
            Thread{svr.start()}.start()
        }
        @JvmStatic @AfterAll fun stopServer() {
            svr.stop()
        }
    }

    val client = StreamingServiceExampleGrpc.newBlockingStub(
        ManagedChannelBuilder.forAddress("localhost", 50051).usePlaintext().build()
    )

    @Test fun unaryNoError() {
        val resp = client.unaryCall(unaryRequest{})
        assertEquals("you made a valid request!", resp.msg)
    }

    @Test fun unaryError() {
        val e = assertFailsWith<StatusRuntimeException> {
            client.unaryCall(unaryRequest { errorOut = true })
        }
        assertEquals(e.status.code, INVALID_ARGUMENT)
    }
}
