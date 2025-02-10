/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:45:46 by hubourge          #+#    #+#             */
/*   Updated: 2025/02/10 17:07:53 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_pf_put_conversions(int fd, const char c, va_list arg, int cpt)
{
	char	*base;

	base = "";
	if (c == 'c')
		return (cpt + ft_pf_putchar_fd(va_arg(arg, int), fd));
	if (c == 's')
		return (cpt + ft_pf_putstr_fd(va_arg(arg, char *), fd));
	if (c == 'd' || c == 'i')
		return (ft_pf_putnbr_fd(va_arg(arg, int), fd, cpt));
	if (c == 'T')
		return (ft_pf_putnbr_st_fd(va_arg(arg, long long), fd, cpt));
	if (c == 'p')
		return (ft_pf_address(va_arg(arg, unsigned long long), cpt, fd));
	if (c == 'u')
		return (ft_pf_putnbr_uns_fd(va_arg(arg, unsigned int), fd, cpt));
	base = "0123456789abcdef";
	if (c == 'x')
		return (ft_pf_putnbr_base(va_arg(arg, int), base, \
		ft_pf_strlen(base), cpt, fd));
	base = "0123456789ABCDEF";
	if (c == 'X')
		return (ft_pf_putnbr_base(va_arg(arg, int), base, \
		ft_pf_strlen(base), cpt, fd));
	return (cpt + ft_pf_putchar_fd(c, fd));
}

static int	ft_pf_check_conversion(const char c)
{
	if (c == 'c' || c == 's' || c == 'p' || c == 'd' || c == 'i'
		|| c == 'u' || c == 'T' || c == 'x' || c == 'X' || c == '%')
		return (1);
	return (0);
}

int	ft_pf_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_printf(int fd, const char *str, ...)
{
	va_list	arg;
	int		i;
	int		cpt;

	i = 0;
	cpt = 0;
	va_start(arg, str);
	if (!str)
		return (-1);
	while (str[i])
	{
		if (str[i] == '%')
		{
			if (str[i + 1] == '\0')
				return (-1);
			if (!ft_pf_check_conversion(str[i + 1]))
				cpt += ft_pf_putchar_fd(str[i], 1);
			cpt = ft_pf_put_conversions(fd, str[++i], arg, cpt);
		}
		else
			cpt += ft_pf_putchar_fd(str[i], fd);
		i++;
	}
	va_end(arg);
	return (cpt);
}
