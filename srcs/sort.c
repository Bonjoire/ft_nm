/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:51:45 by hubourge          #+#    #+#             */
/*   Updated: 2025/02/10 23:16:20 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	sort_symbols(t_data *data, t_symbol *symbols, size_t count)
{
	if (data->opt_p || count == 0)
		return;

	for (size_t i = 0; i < count - 1; i++)
	{
		for (size_t j = 0; j < count - i - 1; j++)
		{
			int cmp_result = ft_strcasecmp_nm(symbols[j].name, symbols[j + 1].name);
			int cmp_addr = symbols[j].address > symbols[j + 1].address;
			int cmp_type = symbols[j].type > symbols[j + 1].type;
			
			if (data->opt_r)
				cmp_result = -cmp_result;

			if (cmp_result > 0 || 
				(cmp_result == 0 && cmp_addr) ||
				(cmp_result == 0 && symbols[j].address == symbols[j + 1].address && cmp_type))
			{
				t_symbol temp = symbols[j];
				symbols[j] = symbols[j + 1];
				symbols[j + 1] = temp;
			}
			if (cmp_result == 0 \
				&& symbols[j].type == symbols[j + 1].type \
				&& (symbols[j].address > symbols[j + 1].address || (symbols[j].address == symbols[j + 1].address)))
			{
				t_symbol temp = symbols[j];
				symbols[j] = symbols[j + 1];
				symbols[j + 1] = temp;
			}
		}
	}
}

// Sort for apple silicon processor
// void	sort_symbols(t_data *data, t_symbol *symbols, size_t count)
// {
// 	if (data->opt_p)
// 		return;

// 	for (size_t i = 0; i < count - 1; i++)
// 	{
// 		for (size_t j = 0; j < count - i - 1; j++)
// 		{
// 			int	cmp_result	= ft_strncmp(symbols[j].name, symbols[j + 1].name, ft_strlen(symbols[j + 1].name) + 1);
// 			int	cmp_addr	= cmp_addr = symbols[j].address > symbols[j + 1].address;
			
// 			// Sort first by name, then by address
// 			if (data->opt_r && (cmp_result < 0 || (cmp_result == 0 && !cmp_addr)))
// 			{
// 				t_symbol temp = symbols[j];
// 				symbols[j] = symbols[j + 1];
// 				symbols[j + 1] = temp;
// 			}
// 			else if (!data->opt_r && (cmp_result > 0 || (cmp_result == 0 && cmp_addr)))
// 			{
// 				t_symbol temp = symbols[j];
// 				symbols[j] = symbols[j + 1];
// 				symbols[j + 1] = temp;
// 			}
// 		}
// 	}
// }