import com.google.protobuf.gradle.*
import com.github.jengelman.gradle.plugins.shadow.tasks.ShadowJar

val coroutinesVersion = "1.6.1"
val grpcVersion = "1.52.1"
val grpcKotlinVersion = "1.3.0"
val protobufVersion = "3.22.0"

plugins {
    application
    kotlin("jvm") version "1.8.10"
    id("com.google.protobuf") version "0.9.2"
    id("idea")
    id("com.github.johnrengelman.shadow") version "8.1.0"
    id("io.ktor.plugin") version "2.2.3"
}

repositories {
    mavenLocal()
    mavenCentral()
}

dependencies {
    implementation(kotlin("stdlib"))
    implementation(kotlin("test"))
    implementation("com.google.protobuf:protobuf-java:${protobufVersion}")
    implementation("com.google.protobuf:protobuf-kotlin:${protobufVersion}")
    implementation("com.google.protobuf:protobuf-java-util:${protobufVersion}")
    implementation("commons-cli:commons-cli:1.5.0")
    implementation("commons-codec:commons-codec:1.15")
    implementation("io.github.microutils:kotlin-logging-jvm:3.0.4:")
    implementation("io.grpc:grpc-kotlin-stub:${grpcKotlinVersion}")
    implementation("io.grpc:grpc-netty-shaded:${grpcVersion}")
    implementation("io.grpc:grpc-protobuf:${grpcVersion}")
    implementation("io.grpc:grpc-services:${grpcVersion}")
    implementation("io.grpc:grpc-stub:${grpcVersion}")
    implementation("org.jetbrains.kotlinx:kotlinx-coroutines-core:${coroutinesVersion}")
    implementation("org.slf4j:slf4j-simple:2.0.6")
    compileOnly("javax.annotation:javax.annotation-api:1.2")
}

protobuf {
    protoc {
        artifact = "com.google.protobuf:protoc:${protobufVersion}"
    }
    plugins {
        id("grpc") {
            artifact = "io.grpc:protoc-gen-grpc-java:${grpcVersion}"
        }
        id("grpckt") {
            artifact = "io.grpc:protoc-gen-grpc-kotlin:${grpcKotlinVersion}:jdk8@jar"
        }
    }
    generateProtoTasks {
        ofSourceSet("main").forEach {
            it.plugins {
                id("grpc")
                id("grpckt")
            }
            it.builtins {
                id("kotlin")
            }
        }
    }
}

application {
    mainClass.set("StreamingExampleKt")
}

ktor {
    fatJar {
        archiveFileName.set("StreamingExample.jar")
    }
}

tasks.withType<ShadowJar> {
    mergeServiceFiles()
}

tasks.test {
    failFast = false
    testLogging.showExceptions = true
    testLogging.exceptionFormat = org.gradle.api.tasks.testing.logging.TestExceptionFormat.FULL
    testLogging.showStackTraces = true
    testLogging.showCauses = true
    // This option leads to relevant logs being shown near the
    // test in the output.
    testLogging.showStandardStreams = true
    maxParallelForks = Runtime.getRuntime().availableProcessors()
    useJUnitPlatform()
}