/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 15:37:27 by etranchi          #+#    #+#             */
/*   Updated: 2019/04/08 15:37:29 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_nm.h"

void				print_otool_64(struct section_64 *section, t_file *file)
{
	int				i;
	int				offset;
	void			*ptr;
	int				j;

	offset = 0;
	i = -1;
	ft_printf("Contents of (__TEXT,__text) section\n");
	while (offset < section->size && offset < file->ptr_size)
	{
		ptr = file->ptr + g_s_v(file->is_swap, section->offset) + offset;
		j = -1;
		ft_printf("%016llx\t", section->addr + offset);
		while (++j < 16)
		{
			print_byte_to_hex(*((char *)file->ptr +
				section->offset + offset + j));
			if (j + offset + 1 >= section->size)
				break ;
		}
		ft_printf("\n");
		offset += 16;
	}
}

void				good_to_print(t_func *lst, t_file *f)
{
	if ((lst->type == 'U' || lst->type == 'u') && f->mode == 64)
		ft_printf("                 ");
	else if ((lst->type == 'U' || lst->type == 'u') && f->mode == 32)
		ft_printf("         ");
	else if (f->mode == 32)
		ft_printf("%08lx ", lst->value);
	else
		ft_printf("%016lx ", lst->value);
	ft_printf("%c ", lst->type);
	ft_printf("%s\n", lst->name);
}

void				print_lst(t_func *lst, t_file *f)
{
	int				i;

	i = -1;
	while (lst)
	{
		get_type(lst, f);
		if (!(lst->type == 'X' || lst->type == 'x' || lst->type == 'u'))
			if (ft_strlen(lst->name) > 0)
				good_to_print(lst, f);
		lst = lst->next;
	}
}

int					print_out(int nsyms, int symoff, int stroff, t_file *f)
{
	int				i;
	char			*stringtable;
	struct nlist_64	*array64;
	struct nlist	*array32;

	i = -1;
	if (f->ptr + symoff > f->ptr + f->ptr_size
		|| f->ptr + stroff > f->ptr + f->ptr_size)
		return (ERROR);
	if (f->mode == 64)
		array64 = (void *)f->ptr + symoff;
	else
		array32 = (void *)f->ptr + symoff;
	stringtable = (void *)f->ptr + stroff;
	while (++i < nsyms)
		if (f->mode == 64 && f->ptr + symoff
			+ (i * sizeof(struct nlist_64)) < f->ptr + f->ptr_size)
			add_to_64(&f->lst, stringtable, array64[i], f);
		else if (f->mode == 32 && f->ptr + symoff
			+ (i * sizeof(struct nlist)) < f->ptr + f->ptr_size)
			add_to_32(&f->lst, stringtable, array32[i], f);
	if (!f->is_fat)
		print_lst(f->lst, f);
	return (SUCCESS);
}
