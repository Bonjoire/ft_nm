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

char * opt_g = " ";
char * opt_u = " ";
char * opt_r = " ";
char * opt_p = " ";
char * opt_valgrind = " ";

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
		// system("sleep 0.2");
		exec_test(ft_nm, nm, files[i]);
		if (check_test_files(ft_nm, nm, files[i], i) == 0)
			continue;
	}
	ft_free_tab(files);
	del_test_files(ft_nm, nm);
}

void parsing_test(int ac, char **av)
{
	if (ac != 6)
	{
		ft_putstr_fd("Usage: ./ft_nm_test g/0 u/0 r/0 p/0 v/0\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	av++;
	while(*av)
	{
		if (*av[0] == '0' || *av[0] == 'g' || *av[0] == 'u' || *av[0] == 'r' || *av[0] == 'p' || *av[0] == 'v')
		{
			if (*av[0] == 'g')
				opt_g = "-g ";
			else if (*av[0] == 'u')
				opt_u = "-u ";
			else if (*av[0] == 'r')
				opt_r = "-r ";
			else if (*av[0] == 'p')
				opt_p = "-p ";
			else if (*av[0] == 'v')
				opt_valgrind = "valgrind ";
		}
		else
		{
			ft_putstr_fd("Usage: ./ft_nm_test g/0 u/0 r/0 p/0 v/0\n", STDERR_FILENO);
			exit(EXIT_FAILURE);
		}
		av++;
	}
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

// Skip similar error message output
int is_error_similar(char *ft_nm, char *nm)
{
	FILE *file_ft_nm = fopen(ft_nm, "r");
	FILE *file_nm = fopen(nm, "r");

    if (!file_ft_nm || !file_nm)
	{
		perror("Erreur d'ouverture");
		return (0);
	}

    char buffer_ft_nm[256];
    char buffer_nm[256];
    fgets(buffer_ft_nm, sizeof(buffer_ft_nm), file_ft_nm);
    fgets(buffer_nm, sizeof(buffer_nm), file_nm);

	if (ft_strnstr(buffer_ft_nm, "file format not recognized", ft_strlen(buffer_ft_nm)) != NULL \
		|| ft_strnstr(buffer_ft_nm, "file too short", ft_strlen(buffer_ft_nm)) != NULL)
	{
		if (ft_strnstr(buffer_nm, "no symbols", ft_strlen(buffer_nm)) != NULL \
			|| ft_strnstr(buffer_nm, "file too short", ft_strlen(buffer_nm)) != NULL \
			|| ft_strnstr(buffer_nm, "not set for section", ft_strlen(buffer_nm)) != NULL \
			|| ft_strnstr(buffer_nm, "file format not recognized", ft_strlen(buffer_nm)) != NULL)
		{
			fclose(file_ft_nm);
			fclose(file_nm);
			return (1);
		}
	}
	fclose(file_ft_nm);
	fclose(file_nm);
	return (0);
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
	
	if (ret == 0 || is_error_similar(ft_nm, nm))
	{
		printf("Test %-3d passed ✅ : %s\n", i, file);
		return (1);
	}
	printf("Test %-3d failed ❌: %s\n", i, file);
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