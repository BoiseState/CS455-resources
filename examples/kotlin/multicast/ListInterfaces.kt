import java.net.NetworkInterface

fun main() {
    for (iface in NetworkInterface.getNetworkInterfaces()) {
        println("----INTERFACE----")
        println("NAME: " + iface.name)
        println("DISPLAY_NAME: " + iface.displayName)
        println("RUNNING: " + iface.isUp)
        println("VIRTUAL: " + iface.isVirtual)
        println("PARENT: " + iface.parent?.name)
        println("INDEX: " + iface.index)
        println("MAXIMUM_TRANSMISSION_UNIT: " + iface.mtu)
        println("LOOPBACK: " + iface.isLoopback)
        println("MULTICAST: " + iface.supportsMulticast())
        println("POINT_TO_POINT: " + iface.isPointToPoint)
        println("----ADDRESSES----")
        for (addr in iface.inetAddresses) {
            println(addr)
        }
        println("----SUBINTERFACES")
        for (sub in iface.subInterfaces) {
            println(sub.name)
        }
        println("\n")
    }
}