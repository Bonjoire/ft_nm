/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 17:10:51 by hubourge          #+#    #+#             */
/*   Updated: 2025/02/10 18:45:42 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

int	file_rights(t_data *data, char *file)
{
	data->fd = open(file, O_RDONLY);
	if (data->fd < 0)
	{
		fprintf(stderr, "nm: %s: Permission denied\n", file);
		return (0);
	}
	if (fstat(data->fd, &(data)->statbuf) < 0)
	{
		perror("fstat");
		free_all_exit(*data, EXIT_FAILURE);
	}
	if (S_ISDIR(data->statbuf.st_mode))
	{
		fprintf(stderr, "nm: Warning: '%s' is a directory\n", file);
		return (0);
	}
	return (1);
}

void	*map_file(t_data *data)
{
	if (data->statbuf.st_size == 0)
		free_all_exit(*data, EXIT_FAILURE);
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
		ft_printf(STDERR_FILENO, "nm: %s: file format not recognized\n", data->file);
		free_all_exit(*data, EXIT_FAILURE);
	}

	// Detect 32 or 64 bits ELF file
	if (e_ident[EI_CLASS] == ELFCLASS64)
		handle64(data);
	else if(e_ident[EI_CLASS] == ELFCLASS32)
		handle32(data);
	else
	{
		ft_printf(STDERR_FILENO, "nm: %s: file format not recognized\n", data->file);
		free_all_exit(*data, EXIT_FAILURE);
	}
}

void	handle64(t_data *data)
{
	// ELF Header and Section Header
	Elf64_Ehdr		*header				= (Elf64_Ehdr *)data->mapped_file;
	Elf64_Shdr		*section_header		= (Elf64_Shdr *)((char *)data->mapped_file + header->e_shoff);
	uint16_t		nb_sections			= header->e_shnum;
	Elf64_Shdr		*section_tab_header	= NULL;
	data->header64						= header;

	check_error64(data, header, nb_sections, NULL);
	section_tab_header			= &section_header[header->e_shstrndx];
	check_error64(data, header, nb_sections, section_tab_header);
	const char	*section_tab	= (const char *)data->mapped_file + section_tab_header->sh_offset;

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
		ft_printf(STDERR_FILENO, "nm: %s: file format not recognized\n", data->file);
		free_all_exit(*data, EXIT_FAILURE);
	}
}

void	check_error64(t_data *data, Elf64_Ehdr *header, uint16_t nb_sections, Elf64_Shdr *section_tab_header)
{
	if (section_tab_header)
	{
		// Check valide section header size
		if (section_tab_header->sh_offset + section_tab_header->sh_size > (size_t)data->statbuf.st_size)
		{
			ft_printf(STDERR_FILENO, "nm: %s: file format not recognized\n", data->file);
			free_all_exit(*data, EXIT_FAILURE);
		}
	}
	else
	{
		// Check if the file is too small
		if ((size_t)data->statbuf.st_size < sizeof(Elf64_Ehdr))
		{
			ft_printf(STDERR_FILENO, "nm: %s: file format not recognized\n", data->file);
			free_all_exit(*data, EXIT_FAILURE);
		}
		// Check if the section header table is within the file
		if ((size_t)header->e_shnum * sizeof(Elf64_Shdr) + (size_t)header->e_shoff > (size_t)data->statbuf.st_size)
		{
			// ft_printf(STDERR_FILENO, "bfd plugin: %s: file too short\n", data->file);
			ft_printf(STDERR_FILENO, "nm: %s: file format not recognized\n", data->file);
			free_all_exit(*data, EXIT_FAILURE);
		}
		// Check if the program header table is within the file
		if ((size_t)header->e_phoff >= (size_t)data->statbuf.st_size || 
			(size_t)header->e_shoff >= (size_t)data->statbuf.st_size)
		{
			ft_printf(STDERR_FILENO, "nm: %s: file format not recognized\n", data->file);
			free_all_exit(*data, EXIT_FAILURE);
		}
		// Check if the section header string table index is valid
		if (header->e_shstrndx >= nb_sections)
		{	
			ft_printf(STDERR_FILENO, "nm: %s: file format not recognized\n", data->file);
			free_all_exit(*data, EXIT_FAILURE);
		}
	}
}

void	handle32(t_data *data)
{
	// ELF Header and Section Header
	Elf32_Ehdr		*header				= (Elf32_Ehdr *)data->mapped_file;
	Elf32_Shdr		*section_header		= (Elf32_Shdr *)((char *)data->mapped_file + header->e_shoff);
	uint16_t		nb_sections			= header->e_shnum;
	Elf32_Shdr		*section_tab_header	= NULL;
	data->header32						= header;

	check_error32(data, header, nb_sections, NULL);
	section_tab_header			= &section_header[header->e_shstrndx];
	check_error32(data, header, nb_sections, section_tab_header);
	const char	*section_tab	= (const char *)data->mapped_file + section_tab_header->sh_offset;

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
		ft_printf(STDERR_FILENO, "nm: %s: file format not recognized\n", data->file);
		free_all_exit(*data, EXIT_FAILURE);
	}
}

void	check_error32(t_data *data, Elf32_Ehdr *header, uint16_t nb_sections, Elf32_Shdr *section_tab_header)
{
	if (section_tab_header)
	{
		// Check valide section header size
		if (section_tab_header->sh_offset + section_tab_header->sh_size > (size_t)data->statbuf.st_size)
		{
			ft_printf(STDERR_FILENO, "nm: %s: file format not recognized\n", data->file);
			free_all_exit(*data, EXIT_FAILURE);
		}
	}
	else
	{
		// Check if the file is too small
		if ((size_t)data->statbuf.st_size < sizeof(Elf32_Ehdr))
		{
			ft_printf(STDERR_FILENO, "nm: %s: file format not recognized\n", data->file);
			free_all_exit(*data, EXIT_FAILURE);
		}
		// Check if the section header table is within the file
		if ((size_t)header->e_shnum * sizeof(Elf32_Shdr) + (size_t)header->e_shoff > (size_t)data->statbuf.st_size)
		{
			ft_printf(STDERR_FILENO, "bfd plugin: %s: file too short\n", data->file);
			ft_printf(STDERR_FILENO, "nm: %s: file format not recognized\n", data->file);
			free_all_exit(*data, EXIT_FAILURE);
		}
		// Check if the program header table is within the file
		if ((size_t)header->e_phoff >= (size_t)data->statbuf.st_size || 
			(size_t)header->e_shoff >= (size_t)data->statbuf.st_size)
		{
			ft_printf(STDERR_FILENO, "nm: %s: file format not recognized\n", data->file);
			free_all_exit(*data, EXIT_FAILURE);
		}
		// Check if the section header string table index is valid
		if (header->e_shstrndx >= nb_sections)
		{	
			ft_printf(STDERR_FILENO, "nm: %s: file format not recognized\n", data->file);
			free_all_exit(*data, EXIT_FAILURE);
		}
	}
}