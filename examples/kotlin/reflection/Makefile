all:

SelfReflect: SelfReflect.kt
	kotlinc SelfReflect.kt -include-runtime -d SelfReflect.jar

ReflectOnAnother: ReflectOnAnother.kt
	kotlinc ReflectOnAnother.kt -include-runtime -d ReflectOnAnother.jar

TraceAncestry: TraceAncestry.kt
	kotlinc TraceAncestry.kt -include-runtime -d TraceAncestry.jar

InvokeMethod: InvokeMethod.kt
	kotlinc InvokeMethod.kt -include-runtime -d InvokeMethod.jar

clean:
	rm *.jar
