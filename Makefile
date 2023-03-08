all: smallFileCombiner
	
smallFileCombiner: smallFileCombiner.cpp
	g++ smallFileCombiner.cpp -o smallFileCombiner

clean:
	rm -f smallFileCombiner
