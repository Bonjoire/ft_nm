/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:34:34 by hubourge          #+#    #+#             */
/*   Updated: 2025/01/29 17:23:24 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

int	main(int ac, char **av)
{
	t_data	data;
	
	init_data(&data);
	parsing(ac, av, &data);
	
	// map_file(&data, av[1]);
	// if (detect_valid_elf(&data, data.mapped_file) == 0)
	// 	free_all_exit(data, EXIT_FAILURE);

	return(EXIT_SUCCESS);
}
