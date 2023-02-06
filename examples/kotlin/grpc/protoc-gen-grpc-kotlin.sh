#!/usr/bin/env sh
PROTOC_LIB_DIR=../build
java -jar $PROTOC_LIB_DIR/protoc-gen-grpc-kotlin-1.3.0-jdk8.jar "$@"
