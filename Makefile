objs = main.o utils.o

out : $(objs)
	cc -g -o out $(objs)
	rm $(objs)

main.o : main.c utils.h
	cc -c -g main.c utils.h

utils.o: utils.c
	cc -c -g utils.c

.PHONY: clean

clean: 
	rm $(objs) out