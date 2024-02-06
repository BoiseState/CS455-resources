CONTRACT_FILE=./StreamingExample.proto
PROTOC_LIB_DIR=../build
LINK_LIB_DIR=../lib

echo "creating servicegen source"
protoc --plugin=protoc-gen-grpckt=./protoc-gen-grpc-kotlin.sh --grpckt_out=. --proto_path=. $CONTRACT_FILE
protoc --plugin=protoc-gen-grpc-java=$PROTOC_LIB_DIR/java-protogen --grpc-java_out=. --proto_path=. $CONTRACT_FILE
protoc --java_out=. --kotlin_out=. $CONTRACT_FILE

echo "creating servicegen jar"
rm servicegen/*.class
javac -cp "$LINK_LIB_DIR/*" servicegen/*.java
jar cf servicegen.jar servicegen/*.class

CHAT_CLASSPATH="."
for jarfile in $LINK_LIB_DIR/*.jar; do
  CHAT_CLASSPATH="$CHAT_CLASSPATH:$jarfile"
done

echo "creating service jar"
kotlinc -classpath "$CHAT_CLASSPATH" StreamingExample.kt servicegen/*.kt servicegen/*.java -include-runtime -d StreamingExample.jar
