all: main

main: main.o locklib.o mydaemon.o
	gcc $^ -o $@

clean:
	rm -fr *.o main
