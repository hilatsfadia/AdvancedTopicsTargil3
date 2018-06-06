CC = g++
LIBS = -ldl

default:
	make all

OBJS = testdcl.o

main: main.o
	$(CC) -rdynamic -o main main.o $(LIBS)

circle.o: circle.c
	g++ -c circle.c -mcmodel=large

square.o: square.c
	g++ -c square.c -mcmodel=large

libcircle.so: circle.o
	g++ -shared -Wl,-soname,libcircle.so -o libcircle.so circle.o

libsquare.so: square.o
	g++ -shared -Wl,-soname,libsquare.so -o libsquare.so square.o

all: main circle.o square.o libcircle.so libsquare.so 

clean:
	rm -f *.so *.o main
