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

#include "../../include/ft_nm.h"

void							put_type(t_section *section, int i,
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

void							get_type(t_func *lst, t_file *file)
{
	lst->tmp_type = lst->type;
	if (ft_strcmp(lst->name, "bad string index") == 0)
	{
		lst->type = 'C';
		if ((lst->tmp_type & N_TYPE) == N_ABS)
			lst->type = 'A';
	}
	else if ((lst->tmp_type & N_TYPE) == N_UNDF)
	{
		lst->type = 'U';
	}
	else if ((lst->tmp_type & N_TYPE) == N_ABS)
		lst->type = 'A';
	else if ((lst->tmp_type & N_TYPE) == N_SECT)
		get_right_section(lst, file);
	else if ((lst->tmp_type & N_TYPE) == N_PBUD)
		lst->type = 'S';
	else if ((lst->tmp_type & N_TYPE) == N_INDR)
		lst->type = 'I';
	else
		lst->type = 'X';
	if (!(lst->tmp_type & N_EXT) && (lst->type != 'X'))
		lst->type += 32;
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
	{
		*lst = sec;
	}
	else
	{
		tmp = *lst;
		while (tmp && tmp->next)
			tmp = tmp->next;
		tmp->next = sec;
	}
	return (SUCCESS);
}

void							put_value_64(struct nlist_64 table,
	t_func *func, t_func **lst, t_file *f)
{
	t_func						*tmp;

	func->type = table.n_type;
	func->value = f->is_swap ? SWAP32(table.n_value) : table.n_value;
	if (!ft_strcmp(func->name, ""))
		func->type = N_UNDF;
	func->sect = table.n_sect;
	func->next = NULL;
	tmp = *lst;
	if (!tmp)
		*lst = func;
	else
		find_best_place(lst, func);
}

void							add_to_64(t_func **lst, char *stringtable,
	struct nlist_64 table, t_file *f)
{
	t_func						*func;
	char						*array_string;
	char						*tmp_name;
	int							swap;

	f->i = -1;
	func = malloc(sizeof(t_func));
	swap = (f->is_swap ? SWAP32(table.n_un.n_strx) : table.n_un.n_strx);
	if (swap > f->offset)
		func->name = ft_strdup("bad string index");
	else
	{
		array_string = stringtable + (swap);
		tmp_name = malloc(sizeof(char) * (f->offset + 1));
		tmp_name[f->offset] = '\0';
		while (++f->i < f->offset && array_string[f->i])
			tmp_name[f->i] = array_string[f->i];
		tmp_name[f->i] = '\0';
		if (ft_strstr(tmp_name, "radr://"))
			return ;
		func->name = malloc(sizeof(char) * (f->i + 1));
		func->name[f->i] = '\0';
		ft_strncpy(func->name, tmp_name, ft_strlen(tmp_name));
	}
	put_value_64(table, func, lst, f);
}

void							init_my_section_64(t_section *sec,
	struct segment_command_64 *seg, int index, struct section_64 *section)
{
	sec->name[seg->nsects] = 0;
	sec->index = index;
	sec->segname = ft_strdup(section->segname);
	sec->next = NULL;
}

int								get_sc_64(struct segment_command_64 *seg,
	t_file *file)
{
	t_section					*sec;
	struct section_64			*section;
	static int					index = 0;
	int							i;

	if ((seg->vmsize < seg->filesize || seg->filesize > file->ptr_size))
		return (error("Corrupted, section size > file size\n"));
	if (!(sec = malloc(sizeof(t_section))))
		return (error("Malloc error."));
	if (!(sec->name = (char **)malloc(sizeof(char*) * (int)(seg->nsects + 1))))
		return (error("Malloc error."));
	section = (struct section_64*)(seg + 1);
	init_my_section_64(sec, seg, ++index, section);
	i = -1;
	while (++i < (file->is_swap ? SWAP32(seg->nsects) : seg->nsects))
	{
		if (!file->nm && !ft_strcmp(section->sectname, "__text"))
			print_otool_64(section, file);
		if (!(sec->name[i] = ft_strdup(section->sectname)))
			return (SUCCESS);
		section++;
	}
	return (add_to_sections(&file->section, sec));
}

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
		sym = (struct symtab_command *)cmd;
		if (cmd && cmd->cmd && (cmd->cmd == LC_SEGMENT_64))
			get_sc_64((struct segment_command_64 *)cmd, f);
		else if (cmd->cmd && (cmd->cmd == LC_SEGMENT))
			get_sc_32((struct segment_command *)cmd, f);
		else if (cmd->cmd && (cmd->cmd == LC_SYMTAB && f->nm))
			if (print_out(g_s_v(f->ppc, sym->nsyms), g_s_v(f->ppc, sym->symoff),
				g_s_v(f->ppc, sym->stroff), g_s_v(f->ppc, sym->strsize), f)
				== ERROR)
				return (error("Error when trying to print"));
		lc_size += f->is_swap ? SWAP32(cmd->cmdsize) : cmd->cmdsize;
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
		f->ncmds = f->is_swap ? SWAP32(header->ncmds) : header->ncmds;
		f->sizeofcmds = f->is_swap ? SWAP32(header->sizeofcmds)
			: header->sizeofcmds;
		f->lc_offset = sizeof(struct mach_header);
	}
	return (dump_segment_commands(f));
}
