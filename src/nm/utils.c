/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 11:15:45 by etranchi          #+#    #+#             */
/*   Updated: 2019/03/21 11:15:46 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../include/ft_nm.h"

int is_magic_64(uint32_t magic) {
  return magic == MH_MAGIC_64 || magic == MH_CIGAM_64;
}


int is_magic_32(uint32_t magic) {
  return magic == MH_MAGIC || magic == MH_CIGAM;
}

int should_swap_bytes(uint32_t magic) {
  return magic == MH_CIGAM || magic == MH_CIGAM_64 || magic == FAT_CIGAM;
}

int is_fat(uint32_t magic) {
  return magic == FAT_MAGIC || magic == FAT_CIGAM;
}

void get_magic(t_file *file) {
	int magic_number;

	magic_number = *(int *)file->ptr;
	file->isFat = is_fat(magic_number);
	file->is64 = is_magic_64(magic_number);
	file->is32 = is_magic_32(magic_number);
	file->isSwap = should_swap_bytes(magic_number);
	// ft_printf("%s 64:%d 32:%d fat:%d swap:%d\n", file->archive_name, file->is64, file->is32, file->isFat, file->isSwap);
	if (file->isFat) {
		// ft_printf("go to fat\n");
		handle_fat_header(file);
	} else if (ft_strncmp(file->ptr, ARMAG, SARMAG) == 0) {
		// ft_printf("go to archive\n");
		handle_archive(file);
	}
	else if (file->is64 || file->is32) {
		// ft_printf("go to 64/32\n");
		handle_header(file);
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
	
	ft_printf("%c%c ", str[1], str[0]);
}


uint32_t	endian_32(uint32_t nb)
{
	nb = ((nb & 0x000000FF) << 24 |
			(nb & 0x0000FF00) << 8 |
				(nb & 0x00FF0000) >> 8 |
					(nb & 0xFF000000) >> 24);
	return (nb);
}

uint64_t	endian_64(uint64_t nb)
{
	nb = ((nb & 0x00000000000000FF) << 56 |
			(nb & 0x000000000000FF00) << 40 |
				(nb & 0x0000000000FF0000) << 24 |
					(nb & 0x00000000FF000000) << 8 |
						(nb & 0x000000FF00000000) >> 8 |
							(nb & 0x0000FF0000000000) >> 24 |
								(nb & 0x00FF000000000000) >> 40 |
									(nb & 0xFF00000000000000) >> 56);
	return (nb);
}