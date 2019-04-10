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

void							put_value_64(struct nlist_64 table,
	t_func *func, t_func **lst, t_file *f)
{
	t_func						*tmp;

	func->type = table.n_type;
	func->value = f->is_swap ? endian_32(table.n_value) : table.n_value;
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

void							add_to_64_2(char *stringtable,
	t_file *f, t_func *func, int swap)
{
	char						*tmp_name;
	char						*array_string;

	array_string = stringtable + swap;
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
	free(tmp_name);
}

void							add_to_64(t_func **lst, char *stringtable,
	struct nlist_64 table, t_file *f)
{
	t_func						*func;
	char						*tmp_name;
	int							swap;

	f->i = -1;
	func = malloc(sizeof(t_func));
	swap = (f->is_swap ? endian_32(table.n_un.n_strx) : table.n_un.n_strx);
	if (swap > f->offset)
		func->name = ft_strdup("bad string index");
	else
	{
		tmp_name = malloc(sizeof(char) * (f->offset + 1));
		tmp_name[f->offset] = '\0';
		while (++f->i < f->offset && (stringtable + swap)[f->i])
			tmp_name[f->i] = (stringtable + swap)[f->i];
		tmp_name[f->i] = '\0';
		if (ft_strstr(tmp_name, "radr://"))
			return ;
		func->name = malloc(sizeof(char) * (f->i + 1));
		func->name[f->i] = '\0';
		ft_strncpy(func->name, tmp_name, ft_strlen(tmp_name));
		free(tmp_name);
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
	while (++i < (file->is_swap ? endian_32(seg->nsects) : seg->nsects))
	{
		if (!file->nm && !ft_strcmp(section->sectname, "__text"))
			print_otool_64(section, file);
		if (!(sec->name[i] = ft_strdup(section->sectname)))
			return (SUCCESS);
		section++;
	}
	return (add_to_sections(&file->section, sec));
}
