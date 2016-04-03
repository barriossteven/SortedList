COMPILER=gcc
CCFLAGS =-g -Wall
AR = ar
all: sl
sl: main.o libsl.a
	$(COMPILER) $(CCLAGS) main.o libsl.a -o sl
libsl.a: sorted-list.o
	$(AR) -r libsl.a sorted-list.o
sorted-list.o: sorted-list.c sorted-list.h
	$(COMPILER) $(CCFLAGS) -c sorted-list.c sorted-list.h
main.o: main.c
	$(COMPILER) $(CCFLAGS) -c main.c
clean:
	rm -f sl
	rm -f *.o
	rm -f *.a
	rm -f *.gch
