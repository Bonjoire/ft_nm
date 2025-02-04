/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:51:45 by hubourge          #+#    #+#             */
/*   Updated: 2025/02/04 19:15:04 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	exec_test(char *ft_nm, char *nm, char *file);
void	check_test_files(char *ft_nm, char *nm, char *file);
void	clear_test_files(char *ft_nm, char *nm);
void	del_test_files(char *ft_nm, char *nm);

char *files[] = {
	"./ft_nm",
	"./test_files/32_bits/32_executable",
	"./test_files/32_bits/32_lib_test.so",
	"./test_files/32_bits/32_main.o",
	"./test_files/64_bits/64_executable",
	"./test_files/64_bits/64_lib_test.so",
	"./test_files/64_bits/64_main.o",
	NULL
};

void	test()
{
	char	*ft_nm	= "ft_nm_output.txt";
	char	*nm	= "nm_output.txt";
	size_t	tab_len = ft_tab_len(files);

	clear_test_files(ft_nm, nm);
	
	for (size_t i = 0; i < tab_len; i++)
	{
		exec_test(ft_nm, nm, files[i]);
		check_test_files(ft_nm, nm, files[i]);
	}

	del_test_files(ft_nm, nm);
}

void	exec_test(char *ft_nm, char *nm, char *file)
{
	char *cmd;
	
	cmd = ft_strjoin("nm ", file);
	cmd = ft_strjoin(cmd, " > ");
	cmd = ft_strjoin(cmd, nm);
	system(cmd);
	free(cmd);

	cmd = ft_strjoin("./ft_nm ", file);
	cmd = ft_strjoin(cmd, " > ");
	cmd = ft_strjoin(cmd, ft_nm);
	system(cmd);
	free(cmd);
}

void	check_test_files(char *ft_nm, char *nm, char *file)
{
	char	*cmd;
	int		ret;

	cmd = ft_strjoin("diff ", ft_nm);
	cmd = ft_strjoin(cmd, " ");
	cmd = ft_strjoin(cmd, nm);
	
	ret = system(cmd);
	free(cmd);
	
	if (ret == 0)
		printf("%s: Test passed ✅\n", file);
	else
		printf("%s: Test failed ❌\n", file);
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