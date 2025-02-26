#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>

void	ft_putchar(char c, int *count)
{
	write(1, &c, 1);
	*count = *count + 1;
}

void	ft_putstr(char *str, int *count)
{
	if (!str)
	{
		ft_putstr("(null)", count);
		return ;
	}

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
		return ;
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

void	print_hex(unsigned int n,  int *count)
{
	unsigned int	rem;

	if (n / 16 != 0)
		print_hex(n / 16, count);
	rem = n % 16;
	if (rem >= 10)
		ft_putchar(rem + 87, count);
	else
		ft_putchar('0' + rem, count);
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

int	main(void)
{
	int	chars;
	chars = ft_printf("remember one simple thing... %s, %d, %x\n", "DON'T PANIC!!!", 42, -42);
	ft_printf("chars: %d\n", chars);
	return (0);
}
