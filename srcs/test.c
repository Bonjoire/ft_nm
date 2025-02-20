/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:51:45 by hubourge          #+#    #+#             */
/*   Updated: 2025/02/20 20:50:45 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

char	*opt_g;
char	*opt_u;
char	*opt_r;
char	*opt_p;
char	*opt_valgrind;
int		total_tests = 0;
int		passed_tests = 0;

void	test(int ac, char **av)
{
	parsing_test(ac, av);

	char	*nm		= TEST_OUTPUT_NM;
	char	*ft_nm	= TEST_OUTPUT_FT_NM;
	char	**files	= get_files(TEST_FILES_PATH);
	size_t	tab_len	= ft_tab_len(files);

	clear_test_files(ft_nm, nm);
	for (size_t i = 0; i < tab_len; i++)
	{
		exec_test(ft_nm, nm, files[i]);
		if (check_test_files(ft_nm, nm, files[i], i) == 0)
			continue;
	}
	ft_free_tab(files);
	del_test_files(ft_nm, nm);
	printf("%d/%d tests passed\n", passed_tests, total_tests);
}

void	parsing_test(int ac, char **av)
{
	if (ac != 6)
		print_help_test();

	char **opt_vars[] = { &opt_g, &opt_u, &opt_r, &opt_p, &opt_valgrind };
	char *valid_opts[] = { "g", "u", "r", "p", "valgrind" };
	char *opt_values[] = { "-g ", "-u ", "-r ", "-p ", "valgrind --track-fds=yes " };

	for (int i = 1; i < 6; i++)
	{
		if (ft_strncmp(av[i], valid_opts[i - 1], ft_strlen(valid_opts[i - 1]) + 1) != 0 && ft_strncmp(av[i], "0", 2) != 0)
			print_help_test();
		if (av[i][0] == '0')
			*opt_vars[i - 1] = " ";
		else
			*opt_vars[i - 1] = opt_values[i - 1];
	}
}

void	print_help_test(void)
{
	ft_putstr_fd("Usage: ./ft_nm_test g/0 u/0 r/0 p/0 valgrind/0\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	exec_test(char *ft_nm, char *nm, char *file)
{
	char	*cmd;
	char	*ft_nm_cmd_part[]	= {"nm ", opt_g, opt_u, opt_r ,opt_p, file, " > ", nm, " 2>&1", NULL};
	char	*nm_cmd_part[]		= {opt_valgrind, "./ft_nm ", opt_g, opt_u, opt_r ,opt_p, file, " > ", ft_nm, " 2>&1", NULL};
	
	cmd = ft_strtab_to_str(ft_nm_cmd_part);
	if (!cmd)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	system(cmd);
	free(cmd);

	cmd = ft_strtab_to_str(nm_cmd_part);
	if (!cmd)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	system(cmd);
	free(cmd);
}

int	check_test_files(char *ft_nm, char *nm, char *file, int i)
{
	char	*cmd;
	int		ret;
	char	*cmd_part[]	= {"diff ", ft_nm, " ", nm, NULL};

	cmd = ft_strtab_to_str(cmd_part);
	if (!cmd)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}	
	ret = system(cmd);
	free(cmd);
	
	total_tests++;
	if (ret == 0)
	{
		printf("Test %-3d passed ✅ : %s\n", i, file);
		passed_tests++;
		return (1);
	}
	printf("Test %-3d failed ❌: %s\n\n", i, file);
	return (0);
}

void	clear_test_files(char *ft_nm, char *nm)
{
	char *cmd;

	cmd = ft_strjoin(">", ft_nm);
	system(cmd);
	free(cmd);
	
	cmd = ft_strjoin(">", nm);
	system(cmd);
	free(cmd);
}

void	del_test_files(char *ft_nm, char *nm)
{
	char *cmd;
	
	cmd = ft_strjoin("rm ", ft_nm);
	system(cmd);
	free(cmd);

	cmd = ft_strjoin("rm ", nm);
	system(cmd);
	free(cmd);
}

char	**get_files(char *path)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**files;
	size_t			i = 0;

	dir = opendir(path);
	if (!dir)
	{
		perror("opendir");
		exit(EXIT_FAILURE);
	}
	files = malloc(sizeof(char *) * 100);
	if (!files)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	while ((entry = readdir(dir)))
	{
		if (entry->d_name[0] != '.')
		{
			char *file_part[] = {path, "/", entry->d_name, NULL};
			files[i] = ft_strtab_to_str(file_part);
			if (!files[i])
			{
				ft_free_tab(files);
				closedir(dir);
				perror("malloc");
				exit(EXIT_FAILURE);
			}
			i++;
		}
		if (i >= 99)
		{
			ft_putstr_fd("Too many files in test directory\n", STDERR_FILENO);
			break;
		}
	}
	files[i] = NULL;
	closedir(dir);
	return (files);
}

char *ft_strtab_to_str(char **tab)
{
	size_t	len		= 0;
	char	*str	= 0;
	size_t	i		= -1;

	while (tab[++i])
		len += ft_strlen(tab[i]);
	str = ft_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	i = -1;
	while (tab[++i])
	{
		ft_strlcat(str, tab[i], len + 1);
		if (!str)
			return (NULL);
	}
	return (str);
}
