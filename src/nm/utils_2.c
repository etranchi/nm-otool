/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 14:58:31 by etranchi          #+#    #+#             */
/*   Updated: 2019/04/10 14:58:32 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_nm.h"

int					is_magic_64(uint32_t magic)
{
	return (magic == MH_MAGIC_64 || magic == MH_CIGAM_64);
}

int					is_magic_32(uint32_t magic)
{
	return (magic == MH_MAGIC || magic == MH_CIGAM);
}

int					should_swap_bytes(uint32_t magic)
{
	return (magic == MH_CIGAM || magic == MH_CIGAM_64 || magic == FAT_CIGAM);
}

int					is_fat(uint32_t magic)
{
	return (magic == FAT_MAGIC || magic == FAT_CIGAM);
}

void				put_type(t_section *section, int i,
	t_file *file, t_func *lst)
{
	if (!section->segname)
		lst->type = 'X';
	if (ft_strcmp(section->name[i], "__bss") == 0)
		lst->type = 'B';
	else if (ft_strcmp(section->name[i], "__data") == 0)
		lst->type = 'D';
	else if (ft_strcmp(section->name[i], "__text") == 0)
		lst->type = 'T';
	else
		lst->type = file->ppc && (lst->type & N_PEXT) ? 'D' : 'S';
	return ;
}
