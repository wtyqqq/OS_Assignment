# Author: Tianyi Wang(tianyiwang58@gmail.com)
# For compiling assignment 1

objs = main.o

myless : $(objs)
	cc -O3 -o myless $(objs)
	rm $(objs)

main.o : main.c
	cc -O3 -c main.c 


.PHONY: clean

clean: 
	rm $(objs) myless