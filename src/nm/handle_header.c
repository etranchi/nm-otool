/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hanlde_header.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 14:59:52 by etranchi          #+#    #+#             */
/*   Updated: 2019/04/10 15:02:06 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_nm.h"

static int						dump_segment_commands(t_file *f)
{
	int							i;
	struct load_command			*cmd;
	struct symtab_command		*sym;
	int							lc_size;

	i = -1;
	lc_size = 0;
	while (++i < f->ncmds)
	{
		cmd = (void *)f->ptr + f->lc_offset + lc_size;
		if (cmd && cmd->cmdsize > f->ptr_size)
			return (error("Corrupted load command\n"));
		if (cmd && cmd->cmd && (cmd->cmd == LC_SEGMENT_64))
			get_sc_64((struct segment_command_64 *)cmd, f);
		else if (cmd->cmd && (cmd->cmd == LC_SEGMENT))
			get_sc_32((struct segment_command *)cmd, f);
		else if (cmd->cmd && (cmd->cmd == LC_SYMTAB && f->nm))
			if (handle_symtab((struct symtab_command *)cmd, f) == ERROR)
				return (error("Error when trying to print"));
		lc_size += f->is_swap ? endian_32(cmd->cmdsize) : cmd->cmdsize;
	}
	return (SUCCESS);
}

int								handle_header(t_file *f)
{
	struct mach_header_64		*header_64;
	struct mach_header			*header;

	header_64 = (void *)f->ptr;
	header = (void *)f->ptr;
	if (!f->nm)
		ft_printf("%s:\n", f->archive_name);
	if (f->is64)
	{
		f->mode = 64;
		f->ncmds = f->is_swap ? endian_64(header->ncmds) : header->ncmds;
		f->sizeofcmds = f->is_swap ? endian_64(header->sizeofcmds)
			: header->sizeofcmds;
		f->lc_offset = sizeof(struct mach_header_64);
	}
	else
	{
		f->mode = 32;
		f->ncmds = f->is_swap ? endian_32(header->ncmds) : header->ncmds;
		f->sizeofcmds = f->is_swap ? endian_32(header->sizeofcmds)
			: header->sizeofcmds;
		f->lc_offset = sizeof(struct mach_header);
	}
	return (dump_segment_commands(f));
}

int								handle_symtab(struct symtab_command *sym,
	t_file *f)
{
	f->offset = g_s_v(f->ppc, sym->strsize);
	if (print_out(g_s_v(f->ppc, sym->nsyms), g_s_v(f->ppc, sym->symoff),
		g_s_v(f->ppc, sym->stroff), f) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

void							find_best_place(t_func **lst, t_func *to_put)
{
	t_func						*tmp;
	t_func						*prev;
	int							ret_cmp;

	tmp = (*lst);
	prev = NULL;
	while (tmp)
	{
		ret_cmp = ft_strcmp(tmp->name, to_put->name);
		if (tmp->name && (ret_cmp > 0 ||
			(ret_cmp == 0 && (to_put->value < tmp->value))))
		{
			if (prev)
				prev->next = to_put;
			else
				(*lst) = to_put;
			to_put->next = tmp;
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	prev->next = to_put;
}

int								add_to_sections(t_section **lst, t_section *sec)
{
	t_section					*tmp;

	if (!(*lst))
		*lst = sec;
	else
	{
		tmp = *lst;
		while (tmp && tmp->next)
			tmp = tmp->next;
		tmp->next = sec;
	}
	return (SUCCESS);
}
