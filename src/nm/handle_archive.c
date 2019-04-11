/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_archive.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 17:11:56 by etranchi          #+#    #+#             */
/*   Updated: 2019/04/08 17:12:46 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_nm.h"

void				perform(struct ar_hdr *header_to_nm, t_file *file)
{
	file->ptr = (void *)header_to_nm;
	get_magic(file);
}

int					put_archive_info(t_file *file, struct ar_hdr *header,
	char *tmp)
{
	if (!ft_strcmp(header->ar_name, "") || (tmp && tmp[0] == '#'))
		return (ERROR);
	ft_printf("%s(%s):\n", file->archive_name, tmp);
	return (SUCCESS);
}

int					check_end(struct ar_hdr *header_to_nm,
	struct ar_hdr *header, t_file *file)
{
	uint32_t offset;

	offset = ft_atoi(header->ar_size) + sizeof(struct ar_hdr);
	if (((void *)header_to_nm - file->ptr + offset
		+ ft_atoi(header_to_nm->ar_size)) > (file->ptr_size))
		return (ERROR);
	return (offset);
}

int					handle_archive(t_file *file)
{
	struct ar_hdr	*header;
	uint32_t		offset;
	struct ar_hdr	*header_to_nm;
	char			**tmp;

	header = (struct ar_hdr *)(file->ptr + SARMAG);
	offset = ft_atoi(header->ar_size) + sizeof(struct ar_hdr);
	header = (struct ar_hdr *)((void *)header + offset);
	ft_printf("Archive : %s\n", file->archive_name);
	while (42)
	{
		if (!ft_strstr(header->ar_name, ARFMAG) ||
			ft_atoi(header->ar_size) == 0)
			return (ERROR);
		tmp = ft_strsplit(header->ar_name, ' ');
		if (put_archive_info(file, header, tmp[0]) == ERROR)
			return (ERROR);
		header_to_nm = (void *)((void *)header +
			sizeof(struct ar_hdr) + ft_atoi(header->ar_size));
		perform(header_to_nm, file);
		if ((offset = check_end(header_to_nm, header, file)) == ERROR)
			return (ERROR);
		header = ((void *)header + offset);
	}
	return (SUCCESS);
}

void				get_type(t_func *lst, t_file *file)
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
