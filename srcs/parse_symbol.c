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
	if (!symtab_section || !strtab_section)
	{
		ft_putstr_fd("Invalid ELF file: Missing .symtab or .strtab\n", STDERR_FILENO);
		free_all_exit(*data, EXIT_FAILURE);
	}

	// Get symbol, string tables
	Elf64_Sym	*symtab		= (Elf64_Sym *)((char *)data->mapped_file + symtab_section->sh_offset);
	char		*strtab		= (char *)data->mapped_file + strtab_section->sh_offset;
	size_t		nb_symbols	= symtab_section->sh_size / sizeof(Elf64_Sym);

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

		if (symbol->st_name == 0 || symbol_name[0] == '\0') // Ignore empty symbol
			continue;
		if (ELF64_ST_TYPE(symbol->st_info) == STT_FILE)		// Ignore file named symbols
			continue;
		// if (symbol->st_shndx >= data->header->e_shnum && symbol->st_shndx != SHN_ABS && symbol->st_shndx != SHN_COMMON) // Ignore invalid section
		// 	continue;

		// Get symbol type
		uint8_t		type			= ELF64_ST_TYPE(symbol->st_info);
		uint8_t		bind			= ELF64_ST_BIND(symbol->st_info); // Local or global
		uint16_t	symbol_index	= symbol->st_shndx;
		char		symbol_type;

		get_set_symbol_type(&symbol_type, type, bind, symbol_index, section_header);
		symbols[symbol_count].name = symbol_name;
		symbols[symbol_count].type = symbol_type;
		symbols[symbol_count].address = symbol_addr;
		symbols[symbol_count].symbol = symbol;
		symbol_count++;
	}

	sort_symbols(symbols, symbol_count);
	for (size_t i = 0; i < symbol_count; i++)
	{
		if (symbols[i].symbol->st_shndx == SHN_UNDEF) 	// Undefined symbol
			printf("%16c %c %s\n", ' ', symbols[i].type, symbols[i].name);
		else											 // Defined symbol
			printf("%016lx %c %s\n", symbols[i].address, symbols[i].type, symbols[i].name);
	}
	free(symbols);
}

void get_set_symbol_type(char *symbol_type, uint8_t type, uint8_t bind, uint16_t symbol_index, Elf64_Shdr *section_header)
{
	if (symbol_index == SHN_UNDEF)		// Weak or Undefined symbol
		*symbol_type = (bind == STB_WEAK) ? 'w' : 'U';
	if (symbol_index == SHN_ABS)		// Absolute symbol
		*symbol_type = 'A';
	if (symbol_index == SHN_COMMON)		// Common symbol
		*symbol_type = 'C';
	if (symbol_index == SHN_UNDEF || symbol_index == SHN_ABS || symbol_index == SHN_COMMON)
		return;

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
        *symbol_type = '?'; // Unknown or unsupported section

    // Handle other weak symbols
    if (bind == STB_WEAK) {
        if (type == STT_OBJECT)
            *symbol_type = (bind == STB_LOCAL) ? 'v' : 'V'; // Weak object
        else
            *symbol_type = (bind == STB_LOCAL) ? 'w' : 'W'; // Weak function
    }
}

void sort_symbols(t_symbol *symbols, size_t count)
{
	for (size_t i = 0; i < count - 1; i++)
	{
		for (size_t j = 0; j < count - i - 1; j++)
		{
			int	cmp_result	= strcmp(symbols[j].name, symbols[j + 1].name);
			int	cmp_addr	= symbols[j].address > symbols[j + 1].address;

			// Sort fisrt by name, then by address
			if (cmp_result > 0 || (cmp_result == 0 && cmp_addr))
			{
				t_symbol temp = symbols[j];
				symbols[j] = symbols[j + 1];
				symbols[j + 1] = temp;
			}
		}
	}
}
