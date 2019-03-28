/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_32.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 11:10:17 by etranchi          #+#    #+#             */
/*   Updated: 2019/03/21 11:10:19 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../include/ft_nm.h"


void print_otool_32(struct section *section, t_file *file) {
	int i;
	int offset;
	void *ptr;
	int j;

	offset = 0;
	i = -1;

	ft_printf("Contents of (__TEXT,__text) section\n");

	while (offset < section->size) {
		ptr = file->ptr + section->offset + offset;
		j = -1;
		ft_printf("%08llx\t", section->addr + offset);
		while (++j < 16){
			print_byte_to_hex(*((char *)file->ptr + section->offset + offset + j));
			if (j + offset + 1 >= section->size)
				break ;
		}
		ft_printf("\n");
		offset += 16;
	}	
}


int get_sc_32(struct segment_command *seg, t_file *file) {
	t_section *sec;
	struct section *section;
	static int index = 0;
	int i;

	if ((seg->vmsize < seg->filesize || seg->filesize > file->ptr_size))
		return (error("Corrupted, section size > file size\n"));
	if (!(sec = malloc(sizeof(t_section))))
		return (error("Malloc error."));
	if (!(sec->name = (char **)malloc(sizeof(char*) * (int) (seg->nsects + 1))))
		return (error("Malloc error."));
	sec->name[seg->nsects] = 0;
	sec->index = index++;
	section = (struct section*)(seg + 1);
	sec->segname = ft_strdup(section->segname);
	sec->next = NULL;
	i = -1;
	while (++i < (file->is_swap ? SWAP32(seg->nsects) : seg->nsects)) {
		if (!file->nm && !ft_strcmp(section->sectname, "__text")) {
			print_otool_32(section, file);
		}
		if (!(sec->name[i] = ft_strdup(section->sectname))) {
			return (SUCCESS);
		}
		section++;
	}
	add_to_sections(&file->section, sec);
	return (SUCCESS);
}

void add_to_32(t_func **lst, char *stringtable, struct nlist table, int offset, t_file *f) {
	t_func *func;
	t_func *tmp;
	char *array_string;
	char *tmp_name;
	int i = -1;
	

	func = malloc(sizeof(t_func));
	if ((f->is_swap ? SWAP32(table.n_un.n_strx) : table.n_un.n_strx) > offset) {
		func->name = ft_strdup("bad string index");
	} else {
		array_string = stringtable + (f->is_swap ? SWAP32(table.n_un.n_strx) : table.n_un.n_strx);
		tmp_name = malloc(sizeof(char) * (offset + 1));
		tmp_name[offset] = '\0';
		while (++i < offset && array_string[i]) {
			tmp_name[i] = array_string[i];
		}
		tmp_name[i] = '\0';
		if (ft_strstr(tmp_name, "radr://")) {
		    return ;
		}
		func->name = malloc(sizeof(char) * (i + 1));
		func->name[i] = '\0';
		ft_strncpy(func->name, tmp_name, ft_strlen(tmp_name));
	}
		
	func->type = table.n_type;
	func->value = f->is_swap ? SWAP32(table.n_value) : table.n_value;
	if(!ft_strcmp(func->name, ""))
		func->type = N_UNDF;
	func->sect = table.n_sect;
	//printf("%s %d\n", func->name, func->sect);
	func->next = NULL;
	tmp = *lst;
	if (!tmp) {
		*lst = func;
	} else {
		find_best_place(lst, func);
	}
}