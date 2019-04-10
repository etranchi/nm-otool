/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 16:23:57 by etranchi          #+#    #+#             */
/*   Updated: 2019/03/28 16:24:01 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_nm.h"

void							put_fat_info(struct fat_arch *arch,
	t_file *file, int n_arch)
{
	if (file->nm)
		ft_printf("%s", file->archive_name);
	if (n_arch > 1)
	{
		if ((file->is_swap ? endian_32(arch->cputype) : arch->cputype)
			== CPU_TYPE_X86)
			ft_printf(" (for architecture i386):\n");
		else
			ft_printf(" (for architecture x86_64):\n");
	}
	else
		ft_printf(":\n");
}

int								perform_header(struct fat_arch *arch,
	t_file *file, void *tmp_ptr)
{
	file->ptr = tmp_ptr +
		(file->is_swap ? endian_32(arch->offset) : arch->offset);
	file->lc_offset = 0;
	file->header_size = (file->is_swap ? endian_32(arch->size) : arch->size);
	return (get_magic(file));
}

int								g_s_v(int isswap, int value)
{
	if (isswap)
		return (endian_32(value));
	else
		return (value);
}

int								handle_fat_header(t_file *file)
{
	struct fat_header			*header;
	struct fat_arch				*arch;
	int							n_arch;
	int							i;

	header = (void *)file->ptr;
	n_arch = g_s_v(file->is_swap, header->nfat_arch);
	arch = (void *)header + sizeof(*header);
	i = -1;
	if (g_s_v(file->is_swap, arch->size) + g_s_v(file->is_swap, arch->offset)
		> file->ptr_size)
		return (error("Corrupted, fat header size + offset > file size\n"));
	while (++i < (n_arch))
	{
		if (g_s_v(file->is_swap, arch->cputype) == CPU_TYPE_POWERPC)
			return (error("Don't handle ppc."));
		if ((g_s_v(file->is_swap, arch->cputype)
			== CPU_TYPE_X86) && n_arch > 1)
			arch++;
		else
			return (perform_header(arch, file, file->to_give_back));
	}
	return (SUCCESS);
}

void							get_right_section(t_func *lst, t_file *file)
{
	t_section					*section;
	int							i;

	section = file->section;
	file->i = 1;
	while (section)
	{
		i = -1;
		while (section->name[++i])
		{
			if (lst->sect == file->i)
				return (put_type(section, i, file, lst));
			file->i++;
		}
		section = section->next;
	}
}
