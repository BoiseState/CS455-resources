buildscript {
    repositories {
        mavenCentral()
    }
    dependencies {
        classpath("org.apache.ivy:ivy:2.5.0")
    }
}

plugins {
    id("de.undercouch.download") version "5.3.1"
}

import de.undercouch.gradle.tasks.download.Download

apply(plugin = "base")

repositories {
    mavenCentral()
}

val toCopy by configurations.creating

dependencies {
    toCopy("org.jetbrains.kotlin:kotlin-stdlib:1.6.21")
    toCopy("org.jetbrains.kotlinx:kotlinx-coroutines-core:1.6.1")
    toCopy("io.grpc:grpc-kotlin-stub:1.3.0")
    toCopy("io.grpc:grpc-stub:1.52.1")
    toCopy("io.grpc:grpc-services:1.52.1")
    toCopy("io.grpc:grpc-netty-shaded:1.52.1")
    toCopy("io.grpc:grpc-protobuf:1.52.1")
    toCopy("io.netty:netty-all:4.1.79.Final")
    toCopy("io.netty:netty-tcnative-boringssl-static:2.0.54.Final")
    toCopy("com.google.protobuf:protobuf-kotlin:3.21.12")
    toCopy("com.google.protobuf:protobuf-java:3.21.12")
}

task<Copy>("fromMaven") {
    from(toCopy)
    into("lib")
}

tasks.register("downloadProtogen") {
    doLast {
        val site = "https://repo1.maven.org/maven2/io/grpc/protoc-gen-grpc-java/1.52.1/"
        val os = System.getProperty("os.name").toLowerCase().replace("\\s+".toRegex(), "")
        val filename = when {
            os.contains("windows") -> "protoc-gen-grpc-java-1.52.1-windows-x86_64.exe"
            os.contains("osx") -> "protoc-gen-grpc-java-1.52.1-osx-x86_64.exe"
            os.contains("linux") -> "protoc-gen-grpc-java-1.52.1-linux-x86_64.exe"
            else -> throw Exception("unrecognized os '$os'")
        }
        val url = site + filename
        download.run {
           src(url)
           dest(File("build", "java-protogen"))
        }
    }
}

tasks.register<Download>("downloadKotlinProtogenJar") {
    src("https://repo1.maven.org/maven2/io/grpc/protoc-gen-grpc-kotlin/1.3.0/protoc-gen-grpc-kotlin-1.3.0-jdk8.jar")
    dest("build")
}

tasks.register("changePermission") {
    dependsOn("downloadProtogen")
    doLast {
        project.exec {
            commandLine("chmod",  "+x", "build/java-protogen")
        }
    }
}

task("download") {
    dependsOn("fromMaven")
    dependsOn("changePermission")
    dependsOn("downloadKotlinProtogenJar")
}
