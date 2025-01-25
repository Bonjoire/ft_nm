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

void	parse_symbols64(t_data *data, Elf64_Shdr *symtab_section, Elf64_Shdr *strtab_section)
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

	for (size_t i = 0; i < nb_symbols; i++)
	{
		// Get symbol name
		Elf64_Sym			*symbol			= &symtab[i];
		char				*symbol_name	= &strtab[symbol->st_name];
		long unsigned int	symbol_addr		= symbol->st_value;

		if (symbol->st_name == 0)
			continue;

		// Extract the type
		uint8_t		type			= ELF64_ST_TYPE(symbol->st_info);
		uint8_t		bind			= ELF64_ST_BIND(symbol->st_info); // Local or global
		uint16_t	symbol_index	= symbol->st_shndx;
		char		symbol_type;

		get_set_symbol(&symbol_type, type, bind, symbol_index, data);

		// Need to sort symbols (by address ?)
		if (symbol_addr)
			printf("%016lx %c %s\n", symbol_addr, symbol_type, symbol_name);
		else
			printf("%16c %c %s\n", ' ', symbol_type, symbol_name);
	}
}

void	get_set_symbol(char *symbol_type, uint8_t type, uint8_t bind, uint16_t symbol_index, t_data *data)
{
	if (symbol_index == SHN_UNDEF)			// Undefined symbol
		*symbol_type = 'U'; 
	else if (symbol_index == SHN_ABS)		// Absolute symbol
		*symbol_type = 'A'; 
	else if (symbol_index == SHN_COMMON)	// Common symbol
		*symbol_type = 'C'; 
	else
	{
		if (type == STT_FUNC)				// Function
			*symbol_type = (bind == STB_LOCAL) ? 't' : 'T'; 
		else if (type == STT_OBJECT)		// Initialized data
			*symbol_type = (bind == STB_LOCAL) ? 'd' : 'D'; 
		else if (type == STT_NOTYPE)		// No specific type
			*symbol_type = (bind == STB_LOCAL) ? 'n' : 'N'; 
		else								// Unknown
			*symbol_type = '?'; 
	}

	if (symbol_index != SHN_UNDEF && symbol_index < ((Elf64_Ehdr *)data->mapped_file)->e_shnum)
	{
		Elf64_Shdr *section = &((Elf64_Shdr *)((char *)data->mapped_file + ((Elf64_Ehdr *)data->mapped_file)->e_shoff))[symbol_index];
		if (section->sh_type == SHT_NOBITS)	// Uninitialized data (.bss)
			*symbol_type = 'B'; 
	}  
}