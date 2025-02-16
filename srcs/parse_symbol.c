/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 17:10:51 by hubourge          #+#    #+#             */
/*   Updated: 2025/01/24 18:11:42 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	parse_symbols64(t_data *data, Elf64_Shdr *symtab_section, Elf64_Shdr *strtab_section, Elf64_Shdr *section_header)
{
	// Get symbol, string tables
	Elf64_Sym	*symtab				= (Elf64_Sym *)((char *)data->mapped_file + symtab_section->sh_offset);
	char		*strtab				= (char *)data->mapped_file + strtab_section->sh_offset;
	size_t		nb_symbols			= symtab_section->sh_size / sizeof(Elf64_Sym);

	// Symbol array
	size_t		symbol_count	= 0;
	t_symbol	*symbols		= malloc(nb_symbols * sizeof(t_symbol));
	if (!symbols)
	{
		perror("malloc");
		free_all_exit(*data, EXIT_FAILURE);
	}

	for (size_t i = 0; i < nb_symbols; i++)
	{
		// Get symbol name and address
		Elf64_Sym			*symbol			= &symtab[i];
		char				*symbol_name	= &strtab[symbol->st_name];
		long unsigned int	symbol_addr		= symbol->st_value;

		// Get symbol type
		uint8_t		type			= ELF64_ST_TYPE(symbol->st_info);
		uint8_t		bind			= ELF64_ST_BIND(symbol->st_info); // Local or global
		uint16_t	symbol_index	= symbol->st_shndx;
		char		symbol_type;

		// Ignore empty symbol/name or file named symbols
		if ((symbol->st_name == 0 || symbol_name[0] == '\0') \
			|| type == STT_FILE) 
			continue;

		// Option -u show only undefined symbols
		if (data->opt_u && symbol_index != SHN_UNDEF)
			continue;

		get_set_symbol_type64(data, &symbol_type, type, bind, symbol_index, section_header);
		symbols[symbol_count].name = symbol_name;
		symbols[symbol_count].type = symbol_type;
		symbols[symbol_count].address = symbol_addr;
		symbols[symbol_count].symbol64 = symbol;
		symbol_count++;
	}

	sort_symbols(data, symbols, symbol_count);
	for (size_t i = 0; i < symbol_count; i++)
	{
		if (symbols[i].symbol64 && symbols[i].symbol64->st_shndx == SHN_UNDEF)	// Undefined symbol
			print_values(16, UNDEF, 0, symbols[i].type, symbols[i].name);
		else																	// Defined symbol
			print_values(16, NOT_UNDEF, symbols[i].address, symbols[i].type, symbols[i].name);
	}
	free(symbols);
}

void	parse_symbols32(t_data *data, Elf32_Shdr *symtab_section, Elf32_Shdr *strtab_section, Elf32_Shdr *section_header)
{
	// Get symbol, string tables
	Elf32_Sym	*symtab		= (Elf32_Sym *)((char *)data->mapped_file + symtab_section->sh_offset);
	char		*strtab		= (char *)data->mapped_file + strtab_section->sh_offset;
	size_t		nb_symbols	= symtab_section->sh_size / sizeof(Elf32_Sym) ;

	// Symbol array
	size_t		symbol_count	= 0;
	t_symbol	*symbols		= malloc(nb_symbols * sizeof(t_symbol));
	if (!symbols)
	{
		perror("malloc");
		free_all_exit(*data, EXIT_FAILURE);
	}

	for (size_t i = 0; i < nb_symbols; i++)
	{
		// Get symbol name and address
		Elf32_Sym			*symbol			= &symtab[i];
		char				*symbol_name	= &strtab[symbol->st_name];
		long unsigned int	symbol_addr		= symbol->st_value;

		// Get symbol type
		uint8_t		type			= ELF32_ST_TYPE(symbol->st_info);
		uint8_t		bind			= ELF32_ST_BIND(symbol->st_info); // Local or global
		uint16_t	symbol_index	= symbol->st_shndx;
		char		symbol_type;
		
		// Ignore empty symbol/name or file named symbols
		if ((symbol->st_name == 0 || symbol_name[0] == '\0') \
			|| type == STT_FILE) 
			continue;

		// Option -u show only undefined symbols
		if (data->opt_u && symbol_index != SHN_UNDEF)
			continue;

		get_set_symbol_type32(data, &symbol_type, type, bind, symbol_index, section_header);
		symbols[symbol_count].name = symbol_name;
		symbols[symbol_count].type = symbol_type;
		symbols[symbol_count].address = symbol_addr;
		symbols[symbol_count].symbol32 = symbol;
		symbol_count++;
	}

	sort_symbols(data, symbols, symbol_count);
	for (size_t i = 0; i < symbol_count; i++)
	{
		if (symbols[i].symbol32 && symbols[i].symbol32->st_shndx == SHN_UNDEF)	// Undefined symbol
			print_values(8, UNDEF, 0, symbols[i].type, symbols[i].name);
		else																	// Defined symbol
			print_values(8, NOT_UNDEF, symbols[i].address, symbols[i].type, symbols[i].name);
	}
	free(symbols);
}

void	get_set_symbol_type64(t_data *data, char *symbol_type, uint8_t type, uint8_t bind, uint16_t symbol_index, Elf64_Shdr *section_header)
{
	if (symbol_index == SHN_UNDEF)		// Weak or Undefined symbol
		*symbol_type = (bind == STB_WEAK) ? 'w' : 'U';
	if (symbol_index == SHN_ABS)		// Absolute symbol
		*symbol_type = (bind == STB_LOCAL) ? 'a' : 'A';
	if (symbol_index == SHN_COMMON)		// Common symbol
		*symbol_type = (bind == STB_LOCAL) ? 'c' : 'C';
	if (!(data->opt_g && bind == STB_LOCAL) && (symbol_index == SHN_UNDEF || symbol_index == SHN_ABS || symbol_index == SHN_COMMON))
		return;

	if (bind == STB_WEAK) {
		if (type == STT_OBJECT)
			*symbol_type = (bind == STB_LOCAL) ? 'v' : 'V'; // Weak object
		else
		{
			*symbol_type = (bind == STB_LOCAL) ? 'w' : 'W';	// Weak function
			if (data->opt_g && bind == STB_LOCAL)
				*symbol_type = 'w';							// Weak function
		}
		return;
	}

	// Option -g show only global symbols
	if (data->opt_g && bind == STB_LOCAL)
	{
		*symbol_type = '0';
		return;
	}

	// Get section table
	Elf64_Shdr *section_tab_header = &section_header[symbol_index];
	if (section_tab_header->sh_type == SHT_NOBITS)
		*symbol_type = (bind == STB_LOCAL) ? 'b' : 'B'; // Uninitialized data (.bss)
	else if (section_tab_header->sh_flags & SHF_EXECINSTR)
		*symbol_type = (bind == STB_LOCAL) ? 't' : 'T'; // Executable code (.text)
	else if (section_tab_header->sh_flags & SHF_WRITE)
		*symbol_type = (bind == STB_LOCAL) ? 'd' : 'D'; // Initialized data (.data)
	else if (section_tab_header->sh_flags & SHF_ALLOC)
		*symbol_type = (bind == STB_LOCAL) ? 'r' : 'R'; // Read-only data (.rodata)
	else if (section_tab_header->sh_type == SHT_DYNAMIC)
		*symbol_type = (bind == STB_LOCAL) ? 'd' : 'D'; // Dynamic section
	else
		*symbol_type = '?';								// Unknown or unsupported section
}

void	get_set_symbol_type32(t_data *data, char *symbol_type, uint8_t type, uint8_t bind, uint16_t symbol_index, Elf32_Shdr *section_header)
{
	if (symbol_index == SHN_UNDEF)		// Weak or Undefined symbol
		*symbol_type = (bind == STB_WEAK) ? 'w' : 'U';
	if (symbol_index == SHN_ABS)		// Absolute symbol
		*symbol_type = (bind == STB_LOCAL) ? 'a' : 'A';
	if (symbol_index == SHN_COMMON)		// Common symbol
		*symbol_type = (bind == STB_LOCAL) ? 'c' : 'C';
	if (!(data->opt_g && bind == STB_LOCAL) && (symbol_index == SHN_UNDEF || symbol_index == SHN_ABS || symbol_index == SHN_COMMON))
		return;

	if (bind == STB_WEAK) {
		if (type == STT_OBJECT)
			*symbol_type = (bind == STB_LOCAL) ? 'v' : 'V'; // Weak object
		else
		{
			*symbol_type = (bind == STB_LOCAL) ? 'w' : 'W';	// Weak function
			if (data->opt_g && bind == STB_LOCAL)
				*symbol_type = 'w';							// Weak function
		}
		return;
	}

	// Option -g show only global symbols
	if (data->opt_g && bind == STB_LOCAL)
	{
		*symbol_type = '0';
		return;
	}

	// Get section table
	Elf32_Shdr *section_tab_header = &section_header[symbol_index];
	if (section_tab_header->sh_type == SHT_NOBITS)
		*symbol_type = (bind == STB_LOCAL) ? 'b' : 'B'; // Uninitialized data (.bss)
	else if (section_tab_header->sh_flags & SHF_EXECINSTR)
		*symbol_type = (bind == STB_LOCAL) ? 't' : 'T'; // Executable code (.text)
	else if (section_tab_header->sh_flags & SHF_WRITE)
		*symbol_type = (bind == STB_LOCAL) ? 'd' : 'D'; // Initialized data (.data)
	else if (section_tab_header->sh_flags & SHF_ALLOC)
		*symbol_type = (bind == STB_LOCAL) ? 'r' : 'R'; // Read-only data (.rodata)
	else if (section_tab_header->sh_type == SHT_DYNAMIC)
		*symbol_type = (bind == STB_LOCAL) ? 'd' : 'D'; // Dynamic section
	else
		*symbol_type = '?';								// Unknown or unsupported section
}
