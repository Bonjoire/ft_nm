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

void parse_symbols64(t_data *data, Elf64_Shdr *symtab_section, Elf64_Shdr *strtab_section)
{
    if (!symtab_section || !strtab_section)
    {
        ft_putstr_fd("Invalid ELF file: Missing .symtab or .strtab\n", STDERR_FILENO);
        free_all_exit(*data, EXIT_FAILURE);
    }

    // Get symbol, string tables
    Elf64_Sym *symtab = (Elf64_Sym *)((char *)data->mapped_file + symtab_section->sh_offset);
    const char *strtab = (const char *)data->mapped_file + strtab_section->sh_offset;

    // Get nb symbols
    size_t num_symbols = symtab_section->sh_size / sizeof(Elf64_Sym);

    printf("Symbol Table:\n");
    printf("  %-50s %-10s %-10s %-10s %-10s\n", "Name", "Value", "Size", "Type", "Binding");

    for (size_t i = 0; i < num_symbols; i++)
    {
        // Get symbol name
        Elf64_Sym *symbol = &symtab[i];
        const char *symbol_name = &strtab[symbol->st_name];

        if (symbol->st_name == 0)
            continue;

        // Extract attributes
        uint8_t type = ELF64_ST_TYPE(symbol->st_info);
        uint8_t bind = ELF64_ST_BIND(symbol->st_info);
        const char *type_str = (type == STT_NOTYPE)  ? "NOTYPE" :
                               (type == STT_OBJECT)  ? "OBJECT" :
                               (type == STT_FUNC)    ? "FUNC" :
                               (type == STT_SECTION) ? "SECTION" :
                               (type == STT_FILE)    ? "FILE" : "UNKNOWN";
        const char *bind_str = (bind == STB_LOCAL)   ? "LOCAL" :
                               (bind == STB_GLOBAL)  ? "GLOBAL" :
                               (bind == STB_WEAK)    ? "WEAK" : "UNKNOWN";

        // Le type de symbole STT_FILE indique qu'il est associé à un fichier source.
        // Il n'est pas un symbole ordinaire comme une fonction ou une variable, 
        // mais plutôt une référence symbolique pour donner des informations 
        // contextuelles sur le fichier source à partir duquel l'objet a été généré.
        if (type == STT_FILE) ///////////////////
            continue;

        printf("  %-50s 0x%08lx 0x%08lx %-10s %-10s\n",
               symbol_name,
               symbol->st_value,
               symbol->st_size,
               type_str,
               bind_str);
    }
}