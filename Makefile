minishell: miniShell.o shellUtils.o utils.o
	gcc $^ -Wall -o $@

%.o: %.c
	gcc $^ -Wall -c

clean:
	rm *.o