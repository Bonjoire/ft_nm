/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:34:27 by hubourge          #+#    #+#             */
/*   Updated: 2025/01/24 17:43:25 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void    free_all_exit(t_data data, int exit_status)
{
    if (data.mapped_file)
        munmap(data.mapped_file, data.statbuf.st_size);
    if (data.fd != -1)
        close(data.fd);
    if (exit_status == EXIT_FAILURE)
        exit(EXIT_FAILURE);
}

void	show_stat(struct stat statbuf)
{
	printf("\n=========== SHOW STAT ===========\n");

    printf("Device ID:                     %ld\n", (long)statbuf.st_dev);
    printf("Inode number:                  %ld\n", (long)statbuf.st_ino);
    printf("File type and mode:            %o\n", statbuf.st_mode);
    printf("Number of hard links:          %ld\n", (long)statbuf.st_nlink);
    printf("User ID of owner:              %d\n", statbuf.st_uid);
    printf("Group ID of owner:             %d\n", statbuf.st_gid);
    printf("Device ID (if special file):   %ld\n", (long)statbuf.st_rdev);
    printf("Total size (bytes):            %lld\n", (long long)statbuf.st_size);
    printf("Block size for filesystem I/O: %ld\n", (long)statbuf.st_blksize);
    printf("Number of blocks allocated:    %lld\n", (long long)statbuf.st_blocks);

    // Affichage des timestamps avec une précision en secondes
    printf("Time of last access:           %s", ctime(&statbuf.st_atime));
    printf("Time of last modification:     %s", ctime(&statbuf.st_mtime));
    printf("Time of last status change:    %s", ctime(&statbuf.st_ctime));

	printf("=================================\n");
}

void show_elf64(Elf64_Ehdr *header)
{
	printf("\n======== SHOW ELF HEADER ========\n");
	
    printf("Magic:   ");
    for (int i = 0; i < EI_NIDENT; i++) {
        printf("%02x ", header->e_ident[i]);
    }
    printf("\n");

    printf("Type:                            %u\n", header->e_type);
    printf("Machine:                         %u\n", header->e_machine);
    printf("Version:                         %u\n", header->e_version);
    printf("Entry point address:             0x%lx\n", header->e_entry);
    printf("Start of program headers:        %lu (bytes into file)\n", header->e_phoff);
    printf("Start of section headers:        %lu (bytes into file)\n", header->e_shoff);
    printf("Flags:                           0x%x\n", header->e_flags);
    printf("Size of this header:             %u (bytes)\n", header->e_ehsize);
    printf("Size of program headers:         %u (bytes)\n", header->e_phentsize);
    printf("Number of program headers:       %u\n", header->e_phnum);
    printf("Size of section headers:         %u (bytes)\n", header->e_shentsize);
    printf("Number of section headers:       %u\n", header->e_shnum);
    printf("Section header string table index: %u\n", header->e_shstrndx);

	printf("================================\n");
}

void    show_section64(Elf64_Shdr *section)
{
    printf("\n======== SHOW SECTION HEADER =======\n");

    printf("Name:                            %u\n", section->sh_name);
    printf("Type:                            %u\n", section->sh_type);
    printf("Flags:                           %lu\n", section->sh_flags);
    printf("Address:                         %lu\n", section->sh_addr);
    printf("Offset:                          %lu\n", section->sh_offset);
    printf("Size:                            %lu\n", section->sh_size);
    printf("Link:                            %u\n", section->sh_link);
    printf("Info:                            %u\n", section->sh_info);
    printf("Address alignment:               %lu\n", section->sh_addralign);
    printf("Entry size:                      %lu\n", section->sh_entsize);

    printf("====================================\n");
}

void    show_symbol64(Elf64_Sym *symbol)
{
    printf("\n======== SHOW SYMBOL =======\n");

    printf("Name:                            %u\n", symbol->st_name);
    printf("Info:                            %u\n", symbol->st_info);
    printf("Other:                           %u\n", symbol->st_other);
    printf("Section index:                   %u\n", symbol->st_shndx);
    printf("Value:                           %lu\n", symbol->st_value);
    printf("Size:                            %lu\n", symbol->st_size);

    printf("=============================\n");
}
