CC = g++ 

CompileParms = -c -Wall -std=c++11 -O3

OBJS = standaard.o aqualin.o main.o

Opdr: $(OBJS)
	$(CC) $(OBJS) -o Aqualin

clean:
	rm -f *.o Aqualin

standaard.o: standaard.cc standaard.h
	$(CC) $(CompileParms)  standaard.cc

aqualin.o: aqualin.cc standaard.h constantes.h aqualin.h
	$(CC) $(CompileParms)  aqualin.cc

main.o: main.cc constantes.h aqualin.h
	$(CC) $(CompileParms)  main.cc

