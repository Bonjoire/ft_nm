/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 16:51:21 by hubourge          #+#    #+#             */
/*   Updated: 2025/01/15 16:40:20 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*str;

	if (size != 0 && nmemb * size / size != nmemb)
		return (NULL);
	str = malloc(nmemb * size);
	if (str == NULL)
		return (NULL);
	ft_memset(str, 0, nmemb * size);
	return (str);
}
