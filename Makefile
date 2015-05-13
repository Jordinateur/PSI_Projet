minishell: miniShell.o shellUtils.o utils.o command.o
	gcc $^ -Wall -g -o $@

%.o: %.c
	gcc $^ -Wall -c -g

clean:
	rm *.o