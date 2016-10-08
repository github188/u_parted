objects = myfunc.o main.o

main:$(objects)
	gcc	-o main $(objects)

myfunc.o: myfunc.c myfunc.h
	gcc	-c myfunc.c

main.o: main.c myfunc.h
	gcc	-c main.c

clean:
	rm	main $(objects) *~ *.txt -rf
