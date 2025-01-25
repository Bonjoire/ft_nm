/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:34:54 by hubourge          #+#    #+#             */
/*   Updated: 2025/01/24 17:40:46 by hubourge         ###   ########.fr       */
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
}           t_data;

// elf_parser.c
void	*map_file(t_data *data, char *file);
int		detect_valid_elf(t_data *data, void *mapped_file);
void	handle_64(t_data *data);
void	parse_symbols64(t_data *data, Elf64_Shdr *symtab_section, Elf64_Shdr *strtab_section);
void	get_set_symbol(char *symbol_type, uint8_t type, uint8_t bind, uint16_t symbol_index, t_data *data);



// utils.c
void    free_all_exit(t_data data, int exit_status);
void	show_stat(struct stat statbuf);
void	show_elf64(Elf64_Ehdr *header);
void    show_section64(Elf64_Shdr *section);
void    show_symbol64(Elf64_Sym *symbol);

#endif