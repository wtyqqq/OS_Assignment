objs = main.o utils.o

out : $(objs)
	cc -o out 	$(objs)
	rm $(objs)

main.o : main.c utils.h
	cc -c main.c utils.h

utils.o: utils.c
	cc -c utils.c

.PHONY: clean

clean: 
	rm $(objs) out