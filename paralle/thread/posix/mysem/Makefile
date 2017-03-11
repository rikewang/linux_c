CFLAGS+=-pthread
LDFLAGS+=-pthread

all: mysem

mysem: main.o mysem.o
	gcc $^ $(CFLAGS) $(LDFLAGS) -o $@ 

clean:
	rm -fr *.o mysem
