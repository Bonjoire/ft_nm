/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:34:34 by hubourge          #+#    #+#             */
/*   Updated: 2025/02/10 17:37:20 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

int	main(int ac, char **av)
{
	if (TEST_MODE)
		test(ac, av);
	else
		nm(ac, av);

	return(EXIT_SUCCESS);
}

void	nm(int ac, char **av)
{
	t_data	data;

	init_data(&data, DO_INIT_OPT);
	parsing(&data, av);
	
	int i = 1;
	while (i < ac || data.nb_files == 0)
	{
		if ((av[i] && av[i][0] != '-') || data.nb_files == 0)
		{
			init_data(&data, DO_NOT_INIT_OPT);
			if (!file_rights(&data, av[i]))
			{
				i++;
				continue;
			}
			if (data.nb_files > 1)
				ft_printf(STDOUT_FILENO, "\n%s:\n", av[i]);

			data.file = av[i];
			if (data.nb_files == 0)
				data.file = "a.out";
			data.nb_files++;
			
			map_file(&data);
			detect_valid_elf(&data, data.mapped_file);
			free_all_exit(data, DO_NOT_EXIT);
		}
		i++;
	}
}
