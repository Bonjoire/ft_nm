/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:34:54 by hubourge          #+#    #+#             */
/*   Updated: 2025/02/02 00:06:23 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <sys/mman.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <elf.h>
# include <errno.h>
# include <string.h>
# include <time.h>

# define DO_NOT_INIT_OPT 0
# define DO_INIT_OPT 1
# define DO_NOT_EXIT 0
# define UNDEF 1
# define NOT_UNDEF 0
# define FILE_TYPE_SHARED_LIB 1
# define FILE_TYPE_OBJECT 2
# define FILE_TYPE_EXECUTABLE 3

typedef struct s_data
{
	void		*mapped_file;
	struct stat	statbuf;
	Elf64_Ehdr	*header64;
	Elf32_Ehdr	*header32;
	size_t		nb_files;
	int			fd;
	int			opt_g;
	int			opt_u;
	int			opt_r;
	int			opt_p;
	int			opt_h;
}				t_data;

typedef struct s_symbol {
	Elf64_Sym		*symbol64;
	Elf32_Sym		*symbol32;
	unsigned long	address;
	char			*name;
	char			type;
}				t_symbol;

// elf_parser.c
void	*map_file(t_data *data, char *file);
void	detect_valid_elf(t_data *data, void *mapped_file);
void	handle64(t_data *data);
void	handle32(t_data *data);

// parse_symbol.c
void	parse_symbols64(t_data *data, Elf64_Shdr *symtab_section, Elf64_Shdr *strtab_section, Elf64_Shdr *section_header);
void	parse_symbols32(t_data *data, Elf32_Shdr *symtab_section, Elf32_Shdr *strtab_section, Elf32_Shdr *section_header);
void	get_set_symbol_type64(t_data *data, char *symbol_type, uint8_t type, uint8_t bind, uint16_t symbol_index, Elf64_Shdr *section_header);
void	get_set_symbol_type32(t_data *data, char *symbol_type, uint8_t type, uint8_t bind, uint16_t symbol_index, Elf32_Shdr *section_header);

// parsing.c
void	parsing(t_data *data, char **av);
int		find_str_set_option(char *str, char *to_find, int *opt, int value);
int		find_char_set_option(char c, char to_find, int *opt, int value);
int		is_option_set(t_data *data, char *str);
void	print_help(void);

// utils.c
void	init_data(t_data *data, int ipt_status);
void    free_all_exit(t_data data, int exit_status);
void	sort_symbols(t_data *data, t_symbol *symbols, size_t count);
void	print_values(int size, int is_undef, long unsigned int address, char type, char *name);

#endif