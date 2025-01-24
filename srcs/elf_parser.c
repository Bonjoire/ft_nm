/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 17:10:51 by hubourge          #+#    #+#             */
/*   Updated: 2025/01/24 18:11:42 by hubourge         ###   ########.fr       */
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
		close(data->fd);
		exit(EXIT_FAILURE);
	}
	
	data->mapped_file = mmap(NULL, data->statbuf.st_size, PROT_READ, MAP_PRIVATE, data->fd, 0);
	if (data->mapped_file == MAP_FAILED)
	{
		perror("mmap");
		close(data->fd);
		exit(EXIT_FAILURE);
	}

	close(data->fd);
	return(data->mapped_file);
}

int	is_valid_efl(void *mapped_file)
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
		Elf64_Ehdr *header = (Elf64_Ehdr *)mapped_file;
        printf("ELF Class: 64-bit\n");
        printf("Entry point: 0x%lx\n", header->e_entry);
        printf("Number of section headers: %d\n", header->e_shnum);
        printf("Number of program headers: %d\n", header->e_phnum);
	}
	else if(e_ident[EI_CLASS] == ELFCLASS32)
	{
		Elf32_Ehdr *header = (Elf32_Ehdr *)mapped_file;
        printf("ELF Class: 32-bit\n");
        printf("Entry point: 0x%x\n", header->e_entry);
        printf("Number of section headers: %d\n", header->e_shnum);
        printf("Number of program headers: %d\n", header->e_phnum);
	}
	else
	{
		ft_putstr_fd("Invalid ELF file\n", STDERR_FILENO);
		return(0);
	}

	return(1);
}
