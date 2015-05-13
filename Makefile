install: shell.o command.o
	gcc $^ -Wall -g -o shell
	rm *.o

%.o: %.c
	gcc $^ -Wall -c -g

clean:
	rm *.o