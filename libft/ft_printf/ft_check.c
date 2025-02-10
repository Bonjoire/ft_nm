/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:43:34 by hubourge          #+#    #+#             */
/*   Updated: 2025/02/10 17:06:16 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_pf_check_int(long n, int cpt, int fd)
{
	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return (cpt + 11);
	}
	else if (n == 2147483647)
	{
		write(fd, "2147483647", 10);
		return (cpt + 10);
	}
	return (cpt);
}

int	ft_pf_check(long n, int cpt, int fd)
{
	cpt = ft_pf_check_int(n, cpt, fd);
	return (cpt);
}
