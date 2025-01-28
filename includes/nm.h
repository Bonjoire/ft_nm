/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:34:54 by hubourge          #+#    #+#             */
/*   Updated: 2025/01/28 14:57:16 by hubourge         ###   ########.fr       */
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

typedef struct s_data
{
	int			fd;
	void		*mapped_file;
	struct stat	statbuf;
	Elf64_Ehdr	*header64;
	Elf32_Ehdr	*header32;

}				t_data;

typedef struct s_symbol {
	Elf64_Sym		*symbol64;
	Elf32_Sym		*symbol32;
	char			*name;
	char			type;
	unsigned long	address;
}				t_symbol;

// elf_parser.c
void	*map_file(t_data *data, char *file);
int		detect_valid_elf(t_data *data, void *mapped_file);
void	handle64(t_data *data);
void	handle32(t_data *data);

// parse_symbol.c
void	parse_symbols64(t_data *data, Elf64_Shdr *symtab_section, Elf64_Shdr *strtab_section, Elf64_Shdr *section_header);
void	parse_symbols32(t_data *data, Elf32_Shdr *symtab_section, Elf32_Shdr *strtab_section, Elf32_Shdr *section_header);
void	get_set_symbol_type64(char *symbol_type, uint8_t type, uint8_t bind, uint16_t symbol_index, Elf64_Shdr *section_header);
void	get_set_symbol_type32(char *symbol_type, uint8_t type, uint8_t bind, uint16_t symbol_index, Elf32_Shdr *section_header);

// utils.c
void    free_all_exit(t_data data, int exit_status);
void	sort_symbols(t_symbol *symbols, size_t count);
void	show_stat(struct stat statbuf);
void	show_elf64(Elf64_Ehdr *header);
void    show_section64(Elf64_Shdr *section);
void    show_symbol64(Elf64_Sym *symbol);

#endif