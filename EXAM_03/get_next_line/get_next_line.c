#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 10
#endif

size_t	ft_strlen(char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_strchr(char *s, char c)
{
	if (!s)
		return (0);
	while (*s)
	{
		if (*s == c)
			return (1);
		s++;
	}
	return (0);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*res;
	size_t	s1_len = 0;
	size_t	s2_len = 0;
	size_t	i = 0;
	size_t	j = 0;
	
	if (s1)
		s1_len = ft_strlen(s1);
	if (s2)
		s2_len = ft_strlen(s2);
	if (!s1_len && !s2_len)
		return (NULL);
	res = (char *)malloc(s1_len + s2_len + 1);
	if (s1)
	{
		while (s1[i])
		{
			res[i] = s1[i];
			i++;
		}
	}
	if (s2)
	{
		while (s2[j])
		{
			res[i] = s2[j];
			i++;
			j++;
		}
	}
	res[i] = '\0';
	if (s1)
		free(s1);
	return (res);
}

char	*extract_line(char *buffer)
{
	char *res;
	size_t	i;

	i = 0;
	while (buffer[i] != '\n')
		i++;
	res = (char *)malloc(i + 2);
	i = 0;
	while (buffer[i] != '\n')
	{
		res[i] = buffer[i];
		i++;
	}
	res[i] = '\n';
	i++;
	res[i] = '\0';
	return (res);
}

void	fix_buffer(char *buffer)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (buffer[i] != '\n' && buffer[i])
		i++;
	i++;
	while (buffer[i])
	{
		buffer[j] = buffer[i];
		j++;
		i++;
	}
	while (buffer[j])
	{
		buffer[j] = '\0';
		j++;
	}
}

char	*get_next_line(int fd)
{
	static	char	buffer[BUFFER_SIZE + 1];
	char	*line = NULL;
	char	*temp = NULL;
	ssize_t	readed_bytes;

	if (fd < 0 && BUFFER_SIZE <= 0)
		return (NULL);

	buffer[BUFFER_SIZE] = '\0';
	
	while (!ft_strchr(temp, '\n'))
	{
		temp = ft_strjoin(temp, buffer);
		fix_buffer(buffer);
		if (ft_strchr(temp, '\n'))
			break;
		readed_bytes = read(fd, buffer, BUFFER_SIZE);
		if (readed_bytes == 0)
		{
			return (temp);
		}
		else if (readed_bytes == -1)
		{
			free(temp);
			return (NULL);
		}
	}
	line = extract_line(temp);
	free(temp);
	return (line);
}

int	main()
{
	int	fd;
	char	*s;

	fd = open("test2", O_RDONLY);
	s = get_next_line(fd);
	while (s)
	{
		printf("%s", s);
		free(s);
		s = get_next_line(fd);
	}
	return (0);
}
