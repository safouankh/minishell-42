all:
	cc *.c libft.a -lreadline -o minishell
clean:
	rm -rf minishell
re: clean all