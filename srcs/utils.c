/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:34:27 by hubourge          #+#    #+#             */
/*   Updated: 2025/02/03 16:23:32 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void    init_data(t_data *data, int opt_status)
{
	data->fd = -1;
	data->mapped_file = NULL;
	data->header64 = NULL;
	data->header32 = NULL;
	if (opt_status == DO_INIT_OPT)
	{
		data->nb_files = 0;
		data->opt_g = 0;
		data->opt_u = 0;
		data->opt_r = 0;
		data->opt_p = 0;
		data->opt_h = 0;
	}
}

void    free_all_exit(t_data data, int exit_status)
{
	if (data.mapped_file)
		munmap(data.mapped_file, data.statbuf.st_size);
	if (data.fd != -1)
		close(data.fd);
	if (exit_status == EXIT_FAILURE)
		exit(EXIT_FAILURE);
}

void	sort_symbols(t_data *data, t_symbol *symbols, size_t count)
{
	if (data->opt_p)
		return;

	for (size_t i = 0; i < count - 1; i++)
	{
		for (size_t j = 0; j < count - i - 1; j++)
		{
			int	cmp_result	= ft_strncmp(symbols[j].name, symbols[j + 1].name, ft_strlen(symbols[j + 1].name) + 1);
			int	cmp_addr	= cmp_addr = symbols[j].address > symbols[j + 1].address;
			
			// Sort first by name, then by address
			if (data->opt_r && (cmp_result < 0 || (cmp_result == 0 && !cmp_addr)))
			{
				t_symbol temp = symbols[j];
				symbols[j] = symbols[j + 1];
				symbols[j + 1] = temp;
			}
			else if (!data->opt_r && (cmp_result > 0 || (cmp_result == 0 && cmp_addr)))
			{
				t_symbol temp = symbols[j];
				symbols[j] = symbols[j + 1];
				symbols[j + 1] = temp;
			}
		}
	}
}

void	print_values(int size, int is_undef, long unsigned int address, char type, char *name)
{
	if (type == '0')
		return;
	if (is_undef)
	{
		for (int i = 0; i < size; i++)
			ft_putchar_fd(' ', 1);
		ft_printf(" %c %s\n", type, name);
	}
	else
	{
		for(int i = ft_addr_len(address, 16); i < size; i++)
			ft_putchar_fd('0', 1);
		ft_putnbr_base_fd(address, "0123456789abcdef", 1);
		ft_printf(" %c %s\n",  type, name);
	}
}
