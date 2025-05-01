#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int		ft_dup(int oldfd);
int		ft_dup2(int oldfd, int newfd);
void		print_error(char *str);
void	error_handler(char *str, char *str2, int exit_flag);
void	execute_command(char **av, char **env, size_t i, int temp_fd);

int		ft_dup(int oldfd)
{
	int	res;

	res = dup(oldfd);
	if (res == -1)
		error_handler("error: fatal", NULL, 1);
	return (res);
}

int		ft_dup2(int oldfd, int newfd)
{
	int	res;

	res = dup2(oldfd, newfd);
	if (res == -1)
		error_handler("error: fatal", NULL, 1);
	return (res);
}

void	print_error(char *str)
{
	while (*str)
	{
		if (write(STDERR_FILENO, str++, 1) == -1)
			error_handler("error: fatal", NULL, 1);
	}
}

void	error_handler(char *str, char *str2, int exit_flag)
{
	if (str)
		print_error(str);
	if (str2)
		print_error(str2);
	write(STDERR_FILENO, "\n", 1);
	if (exit_flag == 1)
		exit(EXIT_FAILURE);
}

void	execute_command(char **av, char **env, size_t i, int temp_fd)
{
	av[i] = NULL;
	ft_dup2(temp_fd, STDIN_FILENO);
	close(temp_fd);
	execve(av[0], av, env);
	error_handler("error: cannot execute ", av[0], 1);
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	size_t	i = 0;
	int		pipe_fds[2];
	int		temp_fd = ft_dup(STDIN_FILENO);
	pid_t	pid;
	while (av[i] && av[i + 1])
	{
		av = av + i + 1;
		i = 0;

		while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
			i++;

		if (!strcmp(av[0], "cd"))
		{
			if (i != 2)
				error_handler("error: cd: bad arguments", NULL, 0);
			else if (chdir(av[1]) == -1)
				error_handler("error: cd: cannot change directory to ", av[1], 0);
		}
		else if (i != 0 && (!av[i] || !strcmp(av[i], ";")))
		{
			pid = fork();
			if (pid == 0)
				execute_command(av, env, i, temp_fd);
			else if (pid == -1)
				error_handler("error: fatal", NULL, 1);
			else
			{
				close(temp_fd);
				while (waitpid(-1, NULL, WUNTRACED) != -1)
					;
				temp_fd = ft_dup(STDIN_FILENO);
			}
		}
		else if (i != 0 && !strcmp(av[i], "|"))
		{
			pipe(pipe_fds);
			pid = fork();
			if (pid == 0)
			{
				ft_dup2(pipe_fds[1], STDOUT_FILENO);
				close(pipe_fds[0]);
				close(pipe_fds[1]);
				execute_command(av, env, i, temp_fd);
			}
			else if(pid == -1)
				error_handler("error: fatal", NULL, 1);
			else
			{
				close(temp_fd);
				close(pipe_fds[1]);
				temp_fd = pipe_fds[0];
			}
		}
	}
	close(temp_fd);
	return (EXIT_SUCCESS);
}
