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
