/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:43:34 by hubourge          #+#    #+#             */
/*   Updated: 2025/02/10 17:07:20 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_pf_putnbr_base(int nbr, char *base, unsigned int len, int cpt, int fd)
{
	char	c;

	c = base[nbr % len];
	if ((nbr / len) > 0)
		cpt = ft_pf_putnbr_adr(nbr / len, base, len, cpt, fd);
	cpt += ft_pf_putchar_fd(c, fd);
	return (cpt);
}
