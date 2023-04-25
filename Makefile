objs = main.o utils.o process_data.o

out : $(objs)
	cc -g -o out $(objs)
	rm $(objs) *.gch

main.o : main.c
	cc -c -g main.c utils.h process_data.h

utils.o: utils.c
	cc -c -g utils.c

process_data.o : process_data.c
	cc -c process_data.c

.PHONY: clean

clean: 
	rm $(objs) out *.gch