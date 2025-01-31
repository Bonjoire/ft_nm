/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_addr_len.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:51:59 by hubourge          #+#    #+#             */
/*   Updated: 2025/01/31 16:52:23 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_addr_len(long unsigned int addr, int base)
{
	int	len;

	len = 1;
	while (addr >= (unsigned long)base)
	{
		addr /= base;
		len++;
	}
	return (len);
}
