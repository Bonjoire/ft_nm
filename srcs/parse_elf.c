/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 17:10:51 by hubourge          #+#    #+#             */
/*   Updated: 2025/01/27 11:41:55 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include 		"nm.h"

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

int	detect_valid_elf(t_data *data, void *mapped_file)
{
	// Check if the file is an ELF file
	unsigned char *e_ident = (unsigned char *)mapped_file;
    if (e_ident[EI_MAG0] != ELFMAG0 ||
        e_ident[EI_MAG1] != ELFMAG1 ||
        e_ident[EI_MAG2] != ELFMAG2 ||
        e_ident[EI_MAG3] != ELFMAG3)
	{
		ft_putstr_fd("Invalid ELF file\n", STDERR_FILENO);
		return(0);
	}

	// Detect 32 or 64 bits ELF file
	if (e_ident[EI_CLASS] == ELFCLASS64)
	{
		handle_64(data);
	}
	else if(e_ident[EI_CLASS] == ELFCLASS32)
	{
		// handle_32(header);
	}
	else
	{
		ft_putstr_fd("Invalid ELF file\n", STDERR_FILENO);
		return(0);
	}

	return(1);
}

// ELF Header
// typedef struct {
// 	unsigned char e_ident[EI_NIDENT];
// 	uint16_t      e_type;
// 	uint16_t      e_machine;
// 	uint32_t      e_version;
// 	ElfN_Addr     e_entry;
// 	ElfN_Off      e_phoff;
// 	ElfN_Off      e_shoff;
// 	uint32_t      e_flags;
// 	uint16_t      e_ehsize;
// 	uint16_t      e_phentsize;
// 	uint16_t      e_phnum;
// 	uint16_t      e_shentsize;
// 	uint16_t      e_shnum;
// 	uint16_t      e_shstrndx;
// } ElfN_Ehdr;

// Section Header
// typedef struct {
// 	uint32_t   sh_name;
// 	uint32_t   sh_type;
// 	uintN_t   sh_flags;
// 	ElfN_Addr sh_addr;
// 	ElfN_Off  sh_offset;
// 	uintN_t   sh_size;
// 	uint32_t   sh_link;
// 	uint32_t   sh_info;
// 	uintN_t   sh_addralign;
// 	uintN_t   sh_entsize;
// } ElfN_Shdr;

// Section 0: 
// Section 1: .text
// Section 2: .data
// Section 3: .bss
// Section 4: .debug_info
// Section 5: .rela.debug_info
// Section 6: .debug_abbrev
// Section 7: .debug_aranges
// Section 8: .rela.debug_aranges
// Section 9: .debug_line
// Section 10: .rela.debug_line
// Section 11: .debug_str
// Section 12: .comment
// Section 13: .note.GNU-stack
// Section 14: .note.gnu.property
// Section 15: .eh_frame
// Section 16: .rela.eh_frame
// Section 17: .symtab
// Section 18: .strtab
// Section 19: .shstrtab

void	handle_64(t_data *data)
{
	// ELF Header and Section Header
	Elf64_Ehdr	*header		= (Elf64_Ehdr *)data->mapped_file;
	Elf64_Shdr	*section_header	= (Elf64_Shdr *)((char *)data->mapped_file + header->e_shoff);
	uint16_t	nb_sections	= header->e_shnum;
	data->header = header;

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

		if (!symtab_section && strcmp(section_name, ".symtab") == 0)
			symtab_section = &section_header[i];
		else if (!strtab_section && strcmp(section_name, ".strtab") == 0)
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
