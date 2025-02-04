/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 17:10:51 by hubourge          #+#    #+#             */
/*   Updated: 2025/02/03 16:22:50 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	*map_file(t_data *data, char *file)
{
	data->fd = open(file, O_RDONLY);
	if (data->fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	if (fstat(data->fd, &(data)->statbuf) < 0)
	{
		perror("fstat");
		free_all_exit(*data, EXIT_FAILURE);
	}
	
	data->mapped_file = mmap(NULL, data->statbuf.st_size, PROT_READ, MAP_PRIVATE, data->fd, 0);
	if (data->mapped_file == MAP_FAILED)
	{
		perror("mmap");
		free_all_exit(*data, EXIT_FAILURE);
	}

	close(data->fd);
	return(data->mapped_file);
}

void	detect_valid_elf(t_data *data, void *mapped_file)
{
	// Check if the file is an ELF file
	unsigned char *e_ident = (unsigned char *)mapped_file;
	if (e_ident[EI_MAG0] != ELFMAG0 ||
		e_ident[EI_MAG1] != ELFMAG1 ||
		e_ident[EI_MAG2] != ELFMAG2 ||
		e_ident[EI_MAG3] != ELFMAG3)
	{
		ft_putstr_fd("Invalid ELF file\n", STDERR_FILENO);
		free_all_exit(*data, EXIT_FAILURE);
	}

	// Detect 32 or 64 bits ELF file
	if (e_ident[EI_CLASS] == ELFCLASS64)
		handle64(data);
	else if(e_ident[EI_CLASS] == ELFCLASS32)
		handle32(data);
	else
	{
		ft_putstr_fd("Invalid ELF file\n", STDERR_FILENO);
		free_all_exit(*data, EXIT_FAILURE);
	}
}

void	handle64(t_data *data)
{
	// ELF Header and Section Header
	Elf64_Ehdr	*header			= (Elf64_Ehdr *)data->mapped_file;
	Elf64_Shdr	*section_header	= (Elf64_Shdr *)((char *)data->mapped_file + header->e_shoff);
	uint16_t	nb_sections		= header->e_shnum;
	data->header64				= header;

	// Table of section_header
	Elf64_Shdr	*section_tab_header	= &section_header[header->e_shstrndx];
	const char	*section_tab		= (const char *)data->mapped_file + section_tab_header->sh_offset;

	// Parse sections symbols;
	Elf64_Shdr	*symtab_section	= NULL;
	Elf64_Shdr	*strtab_section	= NULL;

	// Find .symtab and .strtab sections
	for (uint16_t i = 0; i < nb_sections; i++)
	{
		const char	*section_name = &section_tab[section_header[i].sh_name];

		if (!symtab_section && ft_strncmp(section_name, ".symtab", ft_strlen(".symtab") + 1) == 0)
			symtab_section = &section_header[i];
		else if (!strtab_section && ft_strncmp(section_name, ".strtab", ft_strlen(".strtab") + 1) == 0)
			strtab_section = &section_header[i];
		if (symtab_section && strtab_section)
			break;
	}

	if (symtab_section && strtab_section)
		parse_symbols64(data, symtab_section, strtab_section, section_header);
	else
	{
		ft_putstr_fd("Invalid ELF file: Missing .symtab or .strtab\n", STDERR_FILENO);
		free_all_exit(*data, EXIT_FAILURE);
	}
}

void	handle32(t_data *data)
{
	// ELF Header and Section Header
	Elf32_Ehdr	*header			= (Elf32_Ehdr *)data->mapped_file;
	Elf32_Shdr	*section_header	= (Elf32_Shdr *)((char *)data->mapped_file + header->e_shoff);
	uint16_t	nb_sections		= header->e_shnum;
	data->header32				= header;

	// Table of section_header
	Elf32_Shdr	*section_tab_header	= &section_header[header->e_shstrndx];
	const char	*section_tab		= (const char *)data->mapped_file + section_tab_header->sh_offset;

	// Parse sections symbols;
	Elf32_Shdr	*symtab_section	= NULL;
	Elf32_Shdr	*strtab_section	= NULL;

	// Find .symtab and .strtab sections
	for (uint16_t i = 0; i < nb_sections; i++)
	{
		const char	*section_name = &section_tab[section_header[i].sh_name];

		if (!symtab_section && ft_strncmp(section_name, ".symtab", ft_strlen(".symtab") + 1) == 0)
			symtab_section = &section_header[i];
		else if (!strtab_section && ft_strncmp(section_name, ".strtab", ft_strlen(".strtab") + 1) == 0)
			strtab_section = &section_header[i];
		if (symtab_section && strtab_section)
			break;
	}

	if (symtab_section && strtab_section)
		parse_symbols32(data, symtab_section, strtab_section, section_header);
	else
	{
		ft_putstr_fd("Invalid ELF file: Missing .symtab or .strtab\n", STDERR_FILENO);
		free_all_exit(*data, EXIT_FAILURE);
	}
}
