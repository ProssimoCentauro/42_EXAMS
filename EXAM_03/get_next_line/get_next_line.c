#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 10
#endif


size_t	ft_strlen(char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_strchr(char *str, char c)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == c)
			return (1);
		str++;
	}
	return (0);
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	len_s1 = ft_strlen(s1);
	size_t	len_s2 = ft_strlen(s2);
	char	*res;
	size_t	i = 0;
	
	if (len_s1 == 0 && len_s2 == 0)
		return (NULL);
	res = (char *)malloc(len_s1 + len_s2 + 1);
	if (s1)
	{
		while (s1[i])
		{
			res[i] = s1[i];
			i++;
		}
	}
	while (*s2 && s2 != NULL)
        {
                res[i] = *s2;
                i++;
                s2++;
        }
	if (s1)
		free (s1);
	res[i] = '\0';
	return (res);
}

char	*extract_line(char *buffer)
{
	size_t	i = 0;
	size_t	j = 0;
	char	*res;

	while (buffer[j] != '\n')
		j++;
	res = (char *)malloc(j + 2);
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
	while (buffer[i] && buffer[i] != '\n')
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
	static char buffer[BUFFER_SIZE + 1];
	char *line = NULL;
	int	readed_bytes;
	char	*temp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);

	buffer[BUFFER_SIZE] = '\0';
	while (!ft_strchr(buffer, '\n'))
	{
		line = ft_strjoin(line, buffer);
		fix_buffer(buffer);
		readed_bytes = read(fd, buffer, BUFFER_SIZE);
		if (readed_bytes == -1)
		{
			free(line);
			return (NULL);
		}
		if (readed_bytes == 0)
		{
			if (line && line[0] == '\0')
			{
				free(line);
				return (NULL);
			}
			return (line);
		}
	}
	temp = extract_line(buffer);
	line = ft_strjoin(line, temp);
	free(temp);
	fix_buffer(buffer);
	return (line);
}

int	main()
{
	int	fd = open("test2", O_RDONLY);
	char *str;
	str = get_next_line(fd);


	int	i = 3;
	while (str)
	{
		printf("%s", str);
		free(str);
		str = get_next_line(fd);
		i--; 
	}
	return (0);
}
