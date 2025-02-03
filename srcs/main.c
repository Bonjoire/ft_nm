/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:34:34 by hubourge          #+#    #+#             */
/*   Updated: 2025/02/03 15:32:20 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

int	main(int ac, char **av)
{
	t_data	data;
	(void)ac;

	init_data(&data, DO_INIT_OPT);
	parsing(&data, av);
	
	int i = 1;
	while (i < ac || data.nb_files == 0)
	{
		if ((av[i] && av[i][0] != '-') || data.nb_files == 0)
		{
			init_data(&data, DO_NOT_INIT_OPT);
			
			if (data.nb_files > 1)
			{
				ft_putstr_fd("\n", 1);
				ft_putstr_fd(av[i], 1);
				ft_putstr_fd(":\n", 1);
			}

			char *file = av[i];
			if (data.nb_files == 0)
				file = "a.out";
			data.nb_files++;
			
			map_file(&data, file);
			detect_valid_elf(&data, data.mapped_file);
			free_all_exit(data, DO_NOT_EXIT);
		}
		i++;
	}

	return(EXIT_SUCCESS);
}
