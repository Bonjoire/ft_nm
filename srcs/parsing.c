/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 11:14:29 by hubourge          #+#    #+#             */
/*   Updated: 2025/01/29 11:56:54 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	print_help(void)
{
	ft_putstr_fd("Usage: ./ft_nm [option(s)] [file(s)]\n", 2);
	ft_putstr_fd("	List symbols in [file(s)] (a.out by default).\n", 2);
	ft_putstr_fd("	The options are:\n", 2);
	ft_putstr_fd(" 		-h, --help\tDisplay this information\n", 2);
	ft_putstr_fd("		-a, --debug-syms       Display debugger-only symbols\n", 2);
	ft_putstr_fd("		-g, --extern-only      Display only external symbols\n", 2);
	ft_putstr_fd("		-u, --undefined-only   Display only undefined symbols\n", 2);
	ft_putstr_fd("		-r, --reverse-sort     Reverse the sense of the sort\n", 2);
	ft_putstr_fd("		-p, --no-sort          Do not sort the symbols\n", 2);
}

int	is_option(char *str)
{
	int i = 1;
	
	if (str[i] == '-') 
	{
		i++;
		if (!str[i]) // Ingone --
			return (1);
		if (ft_strncmp(str, "--debug-syms", ft_strlen("--debug-syms") + 1) == 0 \
			|| ft_strncmp(str, "--extern-only", ft_strlen("--extern-only") + 1) == 0 \
			|| ft_strncmp(str, "--undefined-only", ft_strlen("--undefined-only") + 1) == 0 \
			|| ft_strncmp(str, "--reverse-sort", ft_strlen("--reverse-sort") + 1) == 0 \
			|| ft_strncmp(str, "--no-sort", ft_strlen("--no-sort") + 1) == 0 \
			|| ft_strncmp(str, "--help", ft_strlen("--help") + 1) == 0)
			return (1);
	}
	else
	{
		while (str[i])
		{
			if (str[i] == 'a' || str[i] == 'g' || str[i] == 'u' || str[i] == 'r' || str[i] == 'p' || str[i] == 'h')
				i++;
			else
				return (0);
		}
		return (1);
	}
	
	return (0);
}

void	parsing(int ac, char **av, t_data *data)
{
	(void)ac;
	(void)av;
	(void)data;

	int i = 1;
	while (av[i])
	{
		if (av[i][0] == '-')
		{
			if (is_option(av[i]) == 0)
			{
				ft_putstr_fd("ft_nm: unrecognized option ", 2);
				ft_putstr_fd(av[i], 2);
				ft_putstr_fd("\n", 2);
				print_help();
				exit(EXIT_FAILURE);
			}
		}
		i++;
	}
}