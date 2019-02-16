/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hanlde_64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/11 12:38:02 by etranchi          #+#    #+#             */
/*   Updated: 2019/02/11 12:38:04 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../include/ft_nm.h"

int is_magic_64(uint32_t magic) {
  return magic == MH_MAGIC_64 || magic == MH_CIGAM_64;
}

int should_swap_bytes(uint32_t magic) {
  return magic == MH_CIGAM || magic == MH_CIGAM_64 || magic == FAT_CIGAM;
}

int is_fat(uint32_t magic) {
  return magic == FAT_MAGIC || magic == FAT_CIGAM;
}

void	ft_putnbr_hex(int octet, int rem)
{
	char const *base = "0123456789abcdef";

	if (rem > 1)
		ft_putnbr_hex(octet >> 2, rem - 1);
	write(1, base + (octet % 16), 1);
}

void	sp_putchar(unsigned char const *ptr)
{
	char const c = *ptr;

	if (' ' <= c && c <= '~')
		write(1, ptr, 1);
	else
		write(1, ".", 1);
}

void	print_memory(const void *addr, size_t size)
{
	size_t i;
	size_t a;
	unsigned char const *ptr = addr;

	i = 0;
	while (i < size)
	{
		a = 0;
		while (a < 16 && a + i < size)
		{
			ft_putnbr_hex(*(ptr + i + a), 2);
			write(1, " ", 1);
			a++;
		}
		while (a < 16)
		{
			write(1, "  ", 2);
			if (a % 2)
				write(1, " ", 1);
			a++;
		}
		i += 16;
	}
}
void hexDump (void *addr, int len) {
    int i;
    unsigned char buff[17];       // stores the ASCII data
    unsigned char *pc = addr;     // cast to make the code cleaner.


    // Process every byte in the data.

    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.

            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.

            printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.

        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.

        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

}

void				print_byte_to_hex(char byte)
{
	char			str[2];
	short			count;
	short			char_hex;
	unsigned char	cast;

	cast = (unsigned char)byte;
	count = -1;
	while (++count != 2)
	{
		char_hex = cast % 16;
		cast /= 16;
		if (char_hex < 10)
			str[count] = char_hex + '0';
		else
			str[count] = (char_hex % 10) + 'a';
	}
	
	printf("%c%c ", str[1], str[0]);
}


void print_otool_64(struct section_64 *section, t_file *file) {
	int i;
	int offset;
	void *ptr;
	int j;

	offset = 0;
	i = -1;

	printf("Contents of (__TEXT,__text) section\n");

	while (offset < section->size) {
		ptr = file->ptr + section->offset + offset;
		j = -1;
		printf("0000000%llx\t", section->addr + offset);
		while (++j < 16){
			print_byte_to_hex(*((char *)file->ptr + section->offset + offset + j));
			if (j + offset + 1 >= section->size)
				break ;
		}
		printf("\n");
		offset += 16;
	}	
}

void get_sc_64(struct segment_command_64 *seg, t_file *file) {
	t_section *sec;
	struct section_64 *section;
	static int index = 1;
	int i;
	if (!(sec = malloc(sizeof(t_section))))
		return ;
	if (!(sec->name = (char **)malloc(sizeof(char*) * (int) (seg->nsects + 1))))
		return ;
	sec->name[seg->nsects] = 0;
	sec->index = index++;
	section = (struct section_64*)(seg + 1);
	sec->next = NULL;
	i = -1;
	while (++i < (int)seg->nsects) {
		if (!file->nm && !ft_strcmp(section->sectname, "__text")) {
			print_otool_64(section, file);
		}
		if (!(sec->name[i] = ft_strdup(section->sectname))) {
			return;
		}
		section++;
	}
	addToSections(&file->section, sec);
	return ;
}

void get_sc_32(struct segment_command *seg, t_file *file) {
	t_section *sec;
	struct section *section;
	static int index = 1;
	int i;
	if (!(sec = malloc(sizeof(t_section))))
		return ;
	if (!(sec->name = (char **)malloc(sizeof(char*) * (int) (seg->nsects + 1))))
		return ;
	sec->name[seg->nsects] = 0;
	sec->index = index++;
	section = (struct section*)(seg + 1);
	sec->next = NULL;
	i = -1;
	while (++i < (int)seg->nsects) {
		if (!(sec->name[i] = ft_strdup(section->sectname))) {
			return;
		}
		section++;
	}
	addToSections(&file->section, sec);
	return ;
}
