all:


ObjectGraph: ObjectGraph.kt
	kotlinc ObjectGraph.kt -include-runtime -d ObjectGraph.jar

CryogenicsV1: Cryogenics.kt MioAlmaV1.kt
	kotlinc Cryogenics.kt MioAlmaV1.kt -include-runtime -d CryogenicsV1.jar

CryogenicsV2: Cryogenics.kt MioAlmaV2.kt
	kotlinc Cryogenics.kt MioAlmaV2.kt -include-runtime -d CryogenicsV2.jar

CryogenicsV3: Cryogenics.kt MioAlmaV3.kt
	kotlinc Cryogenics.kt MioAlmaV3.kt -include-runtime -d CryogenicsV3.jar

SerHashTable: SerHashTable.kt
	kotlinc SerHashTable.kt -include-runtime -d SerHashTable.jar

clean:
	rm *.jar *.ser
