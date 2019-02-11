compile: helloWorld.c addVectors.c
	gcc -o helloWorld helloWorld.c -lm -O3 -lpthread
	gcc -o addVectors addVectors.c -lm -O3 -lpthread
	gcc -o first first.c -lm -O3 -lpthread
	gcc -o second second.c -lm -O3 -lpthread

clean:
	rm helloWorld addVectors
