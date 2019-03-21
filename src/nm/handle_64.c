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

#define SWAP32(x) ((((x) & 0xff000000) >> 24) | (((x) & 0xff0000) >> 8) | (((x) & 0xff00) << 8) | (((x) & 0xff) << 24))

void print_otool_64(struct section_64 *section, t_file *file) {
	int i;
	int offset;
	void *ptr;
	int j;

	offset = 0;
	i = -1;

	ft_printf("Contents of (__TEXT,__text) section\n");

	while (offset < section->size) {
		ptr = file->ptr + (file->isSwap ? SWAP32(section->offset) : section->offset) + offset;
		j = -1;
		ft_printf("%016llx\t", section->addr + offset);
		while (++j < 16){
			print_byte_to_hex(*((char *)file->ptr + section->offset + offset + j));
			if (j + offset + 1 >= section->size)
				break ;
		}
		ft_printf("\n");
		offset += 16;
	}	
}

void get_sc_64(struct segment_command_64 *seg, t_file *file) {
t_section *sec;
	struct section_64 *section;
	static int index = 1;
	int i;

	if ((seg->vmsize < seg->filesize || seg->filesize > file->buff_size) && file->nm ) {
		ft_printf("Corrupted\n");
		exit(1);
	}
	if (!(sec = malloc(sizeof(t_section))))
		return ;
	if (!(sec->name = (char **)malloc(sizeof(char*) * (int)(seg->nsects + 1))))
		return ;
	sec->name[seg->nsects] = 0;
	sec->index = index++;
	section = (struct section_64*)(seg + 1);
	sec->segname = ft_strdup(section->segname);
	sec->next = NULL;
	i = -1;
	while (++i < (file->isSwap ? SWAP32(seg->nsects) : seg->nsects)) {
		if (!file->nm && !ft_strcmp(section->sectname, "__text")) {
			print_otool_64(section, file);
		}
		if (!(sec->name[i] = ft_strdup(section->sectname))) {
			return;
		}
		section++;
	}
	addToSections(&file->section, sec);
	return ;
}






void get_magic(t_file *file);


void get_right_section(t_func *lst, t_file *file) {
	t_section *section;
	int i;
	int index;
	section = file->section;
	index = 1;
	while(section)
	{
		i = -1;
		while (section->name[++i]){
			if(lst->sect == index)
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
			index++;
		}
		section = section->next;
	}
}

void getType(t_func *lst, t_file *file) {
	lst->tmp_type = lst->type;
	if (ft_strcmp(lst->name, "bad string index") == 0) {
		lst->type = 'C';
		if ((lst->tmp_type & N_TYPE ) == N_ABS)
			lst->type = 'A';
	}
	else if ((lst->tmp_type & N_TYPE) == N_UNDF) {
		lst->type = 'U';
	}
	else if ((lst->tmp_type & N_TYPE) == N_ABS) {
		lst->name = "";
		lst->type = 'A';
	}
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


void find_best_place(t_func **lst, t_func *to_put) {
	t_func *tmp;
	t_func *prev;

	tmp = (*lst);
	prev = NULL;
	while (tmp) {
		if (tmp->name && 
			(ft_strcmp(tmp->name, to_put->name) > 0 || (ft_strcmp(tmp->name, to_put->name) == 0 && (to_put->value < tmp->value)))) {
			if (prev) {
				prev->next = to_put;
				to_put->next = tmp;
			} else {
				to_put->next = tmp;
				(*lst) = to_put;
			}
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	prev->next = to_put;
}





void addToSections(t_section **lst, t_section *sec) {
	t_section * tmp;

	if (!(*lst)) {
		*lst = sec;
		return;
	} else {
		tmp = *lst;
		while (tmp && tmp->next) 
			tmp = tmp->next;
		tmp->next = sec;
	}
}


void print_lst(t_func *lst, t_file *f) {
	t_func *tmp;
	t_func *next;
	t_section *section;
	t_section *n_section;
	int i = -1;


	tmp = lst;
	while (lst) {
		getType(lst, f);
		if (!(lst->type == 'X' || lst->type == 'x' || (lst->type == 'u' && lst->value == 0 && ft_strlen(lst->name) > 0)) ) {
			if (ft_strlen(lst->name) > 0) {
				if ((lst->type == 'U' || lst->type == 'u') && f->mode == 64) {
					ft_printf("                 ");
				}
				else if ((lst->type == 'U' || lst->type == 'u') && f->mode == 32)
					ft_printf("         ");
				else if (f->mode == 32)
					ft_printf("%08lx ", lst->value);
				else {
					ft_printf("%016lx ", lst->value);
				}
				ft_printf("%c ", lst->type);	
				ft_printf("%s\n", lst->name);
			}
		}
		lst = lst->next;
	}
}

void print_out(int nsyms, int symoff, int stroff, int strsize, t_file *f) {
	int i;
	char *stringtable;
	struct nlist_64 *array64;
	struct nlist 	*array32;
	i = -1;


	if (f->ptr + symoff > f->ptr + f->ptr_size || f->ptr + stroff > f->ptr + f->ptr_size ) {
		f->corrupted = 1;
		printf("Corrupted\n");
		return ;
	}
	if (f->mode == 64) 
		array64 = (void *)f->ptr + symoff;
	else  {
		array32 = (void *)f->ptr + symoff;
		f->mode = 32;
	}
	// printf("laaaa %d %d %d %d %d %d\n",nsyms, symoff, stroff, strsize, f->mode, f->ppc);
	stringtable = (void *)f->ptr + stroff;
	while (++i < nsyms){
		if (f->mode == 64 && f->ptr + symoff + (i * sizeof(struct nlist_64)) < f->ptr + f->ptr_size){
			addTo64(&f->lst, stringtable, array64[i], strsize, f);
		}
		else if (f->mode == 32 && f->ptr + symoff + (i * sizeof(struct nlist)) < f->ptr + f->ptr_size) {
			addTo32(&f->lst, stringtable, array32[i], strsize, f);
		}
	}
	if (!f->isFat)
		print_lst(f->lst, f);
}




void addTo64(t_func **lst, char *stringtable, struct nlist_64 table, int offset, t_file *f) {
	t_func *func;
	t_func *tmp;
	char *array_string;
	char *tmp_name;
	int i = -1;
	

	func = malloc(sizeof(t_func));
	if ((f->isSwap ? SWAP32(table.n_un.n_strx) : table.n_un.n_strx) > offset) {
		func->name = ft_strdup("bad string index");
	} else {
		array_string = stringtable + (f->isSwap ? SWAP32(table.n_un.n_strx) : table.n_un.n_strx);
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
	func->value = table.n_value;
	if(!ft_strcmp(func->name, ""))
		func->type = N_UNDF;
	func->sect = table.n_sect;
	func->next = NULL;
	tmp = *lst;
	if (!tmp) {
		*lst = func;
	} else {
		find_best_place(lst, func);
	}
}

static void dump_segment_commands(t_file *f) {
	int i;
	struct load_command *cmd;
	struct symtab_command *sym;
	int lc_size;

	i = -1;
	lc_size = 0;
	while(++i < f->ncmds) {
		cmd = (void *)f->ptr + f->lc_offset + lc_size;
		// if (cmd && cmd->cmdsize > f->buff_size && !f->ppc) {
		// 	ft_printf("Corrupted\n");
		// 	return;
		// }
		if (cmd && cmd->cmd && (cmd->cmd == LC_SEGMENT_64 || (f->isSwap && cmd->cmd == SWAP32(LC_SEGMENT_64)))) {
			// printf("segment 64\n");
			get_sc_64((struct segment_command_64 *)((void *)f->ptr + f->lc_offset + lc_size), f);
		} else if (cmd->cmd && (cmd->cmd == LC_SEGMENT || (f->isSwap && cmd->cmd == SWAP32(LC_SEGMENT)))) {
			// printf("segment 32\n");
			get_sc_32((struct segment_command *)((void *)f->ptr + f->lc_offset + lc_size), f);
		} else  if (cmd->cmd && (cmd->cmd == LC_SYMTAB || (f->isSwap && cmd->cmd == SWAP32(LC_SYMTAB))) && f->nm) {
			sym = (struct symtab_command *) cmd;
			print_out(f->ppc ? SWAP32(sym->nsyms) : sym->nsyms, f->ppc ? SWAP32(sym->symoff) : sym->symoff, f->ppc ? SWAP32(sym->stroff) : sym->stroff, f->ppc ? SWAP32(sym->strsize) : sym->strsize, f);
		}
		lc_size += f->isSwap ? SWAP32(cmd->cmdsize) : cmd->cmdsize;
			
	}

}


void handle_header(t_file *f) {
	if (f->is64) {
		size_t header_size = sizeof(struct mach_header_64);
		struct mach_header_64 *header = (void *)f->ptr;
		f->mode = 64;
		f->ncmds = header->ncmds;
		f->lc_offset = header_size;
		f->sizeofcmds = header->sizeofcmds;
	} else {

		size_t header_size = sizeof(struct mach_header);
		struct mach_header *header = (void *)f->ptr;
		f->mode = 32;
		f->ncmds = f->isSwap ? SWAP32(header->ncmds) : header->ncmds;
		f->sizeofcmds = f->isSwap ? SWAP32(header->sizeofcmds) : header->sizeofcmds;

		f->lc_offset = header_size;

	}
	dump_segment_commands(f);
}





void get_magic(t_file *file) {
	int magic_number;

	magic_number = *(int *)file->ptr;
	file->is64 = is_magic_64(magic_number);
	file->is32 = is_magic_32(magic_number);
	file->isFat = is_fat(magic_number);
	file->isSwap = should_swap_bytes(magic_number);
	if (file->isFat) {
		handle_fat_header(file);
	} else if (ft_strncmp(file->ptr, ARMAG, SARMAG) == 0) {
		handle_archive(file);
	}
	else if (file->is64 || file->is32) {
		handle_header(file);
	}
	else {
		file->corrupted = 1;
		ft_printf("Corrupted.");
	}
}