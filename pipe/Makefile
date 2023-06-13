# Author: Tianyi Wang(tianyiwang58@gmail.com)
# For compiling assignment 1

objs = main.o process_data.o

pipe : $(objs)
	cc -O3 -o pipe $(objs)
	rm $(objs) *.gch

main.o : main.c
	cc -O3 -c main.c process_data.h

process_data.o : process_data.c
	cc -O3 -c process_data.c

.PHONY: clean

clean: 
	rm $(objs) pipe *.gch