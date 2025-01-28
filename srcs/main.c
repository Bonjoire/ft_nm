/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:34:34 by hubourge          #+#    #+#             */
/*   Updated: 2025/01/28 17:43:01 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

int	main(int ac, char **av)
{
	t_data data;

	if(ac != 2)
	{
		ft_putstr_fd("Usage: ./malloc [file]\n", 2);
		return(EXIT_FAILURE);
	}
	
	map_file(&data, av[1]);
	if (detect_valid_elf(&data, data.mapped_file) == 0)
		free_all_exit(data, EXIT_FAILURE);

	
	return(EXIT_SUCCESS);
}
