#include <unistd.h>
#include <stdarg.h>

void	ft_putchar(char c, int *count)
{
	write(1, &c, 1);
	*count = *count + 1;
}

void	ft_putstr(char *str, int *count)
{
	while (*str)
	{
		ft_putchar(*str, count);
		str++;
	}
}

void	ft_putnbr(int n, int *count)
{
	if (n == -2147483648)
	{
		write(1, "-2147483648", 11);
		*count = *count + 11;
	}
	if (n < 0)
	{
		n = -n;
		write(1, "-", 1);
		*count = *count + 1;
	}
	if (n > 9)
		ft_putnbr(n / 10, count);
	ft_putchar((n % 10) + '0', count);
}

void	print_hex(int num, int *count)
{
	char *base = "0123456789abcdef";

	if (num >= 16)
		print_hex(num / 16, count);
	ft_putchar(base[num % 16], count);
}

void	select_type(const char *s, va_list args, int *count)
{
	if (*s == 's')
		ft_putstr(va_arg(args, char *), count);
	else if (*s == 'd')
		ft_putnbr(va_arg(args, int), count);
	else if (*s == 'x')
		print_hex(va_arg(args, int), count);
}

int	ft_printf(const char *s, ... )
{
	va_list	args;
	int	count;

	va_start(args, s);
	count = 0;
	while (*s)
	{
		if (*s == '%')
			select_type(++s, args, &count);
		else
			ft_putchar(*s, &count);
		s++;
	}
	va_end(args);
	return (count);
}

int	main(int ac, char **av)
{
	(void)ac;
	int	chars;
	chars = ft_printf("Hello how are you? %s, %d, %x\n", "DON'T PANIC!!!", 90, 42);
	ft_printf("chars: %d\n", chars);
	return (0);
}
