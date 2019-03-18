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


int is_magic_32(uint32_t magic) {
  return magic == MH_MAGIC || magic == MH_CIGAM;
}

int should_swap_bytes(uint32_t magic) {
  return magic == MH_CIGAM || magic == MH_CIGAM_64 || magic == FAT_CIGAM;
}

int is_fat(uint32_t magic) {
  return magic == FAT_MAGIC || magic == FAT_CIGAM;
}

void				print_byte_to_hex(char byte)
{
	char			str[2];
	short			count;
	short			char_hex;
	unsigned char	cast;

	cast = (unsigned char)byte;
	count = -1;
	while (++count != 2)
	{
		char_hex = cast % 16;
		cast /= 16;
		if (char_hex < 10)
			str[count] = char_hex + '0';
		else
			str[count] = (char_hex % 10) + 'a';
	}
	
	ft_printf("%c%c ", str[1], str[0]);
}


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


void print_otool_64(struct section_64 *section, t_file *file) {
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
	if (!(sec = malloc(sizeof(t_section))))
		return ;
	if (!(sec->name = (char **)malloc(sizeof(char*) * (int) (seg->nsects + 1))))
		return ;
	sec->name[seg->nsects] = 0;
	sec->index = index++;
	section = (struct section_64*)(seg + 1);
	sec->next = NULL;
	i = -1;
	while (++i < (file->isSwap ? SWAP32(seg->nsects) : seg->nsects)) {
		if (!file->nm && !ft_strcmp(section->sectname, "__text")) {
			print_otool_64(section, file);
		}
		ft_printf("%s\n", section->sectname);
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
	while (++i < (file->isSwap ? SWAP32(seg->nsects) : seg->nsects)) {
		if (!file->nm && !ft_strcmp(section->sectname, "__text")) {
			print_otool_32(section, file);
		}
		if (!(sec->name[i] = ft_strdup(section->sectname))) {
			return;
		}
		section++;
	}
	addToSections(&file->section, sec);
	return ;
}



#define SWAP32(x) ((((x) & 0xff000000) >> 24) | (((x) & 0xff0000) >> 8) | (((x) & 0xff00) << 8) | (((x) & 0xff) << 24))

void get_magic(t_file *file);


int	swap_uint16(int n)
{
	n = ((n << 8) & 0xFF00FF00) | ((n >> 8) & 0xFF00FF);
	return ((n << 16) | (n >> 16));
}



void swap_value(t_func *first, t_func *second) {
	char 	*tmp_name;
	int 	tmp_type;
	int 	tmp_sect;
	unsigned long tmp_value;

	tmp_name = first->name;
	tmp_type = first->type;
	tmp_sect = first->sect;
	tmp_value = first->value;
	first->name = second->name;
	first->type = second->type;
	first->sect = second->sect;
	first->value = second->value;
	second->name = tmp_name;
	second->type = tmp_type;
	second->sect = tmp_sect;
	second->value = tmp_value;

}

void			sort_name(t_func **begin)
{
	t_func	*curr;
	t_func	*next;
	char	*tmp;
	t_func *prev;
	t_func *prev_bis;


	curr = (*begin);
	prev = NULL;
	prev_bis = NULL;
	while (curr && curr->next != NULL)
	{
		next = curr->next;
		if (next && (ft_strcmp(curr->name, next->name) > 0))
		{
			swap_value(next, curr);
			curr = (*begin);
		}
		else {
			prev_bis = prev;
			prev = curr;
			curr = curr->next;
		}
	}
}


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
				lst->tmp_type = lst->type;
				lst->type = section->name[i][2];
				if (!ft_strcmp(section->name[i], "__text") && (lst->tmp_type & N_EXT))
					lst->type -= 32;
				if (!ft_strcmp(section->name[i], "__stubs") || !ft_strcmp(section->name[i], "__all_image_info__DATA"))
					lst->type = 'S';
				if (!ft_strcmp(section->name[i], "__data") && (lst->tmp_type & N_EXT))
					lst->type -= 32;
				if (!ft_strcmp(section->name[i], "__interpose") ||!ft_strcmp(section->name[i], "__objc_const") || !ft_strcmp(section->name[i], "__objc_classname") || !ft_strcmp(section->name[i], "__objc_methname") || !ft_strcmp(section->name[i], "__objc_methtype") || !ft_strcmp(section->name[i], "__objc_nlclslist") || !ft_strcmp(section->name[i], "__objc_opt_rw") || !ft_strcmp(section->name[i], "__objc_opt_ro") || !ft_strcmp(section->name[i], "__thread_bss") || !ft_strcmp(section->name[i], "__thread_vars") || !ft_strcmp(section->name[i], "__common") || !ft_strcmp(section->name[i], "__objc_ivar") || !ft_strcmp(section->name[i], "__program_vars") || !ft_strcmp(section->name[i], "__eh_frame") || !ft_strcmp(section->name[i], "__objc_data") || !ft_strcmp(section->name[i], "__gcc_except_tab__TEXT") || !ft_strcmp(section->name[i], "__cstring") || !ft_strcmp(section->name[i], "__crash_info") || !ft_strcmp(section->name[i], "__const") || !ft_strcmp(section->name[i], "__ustring") || !ft_strcmp(section->name[i], "__info_plist") )
					lst->type = 's';
				if (!ft_strcmp(section->name[i], "__thread_vars") && lst->tmp_type & N_PEXT)
					lst->type = 't';
				if ((!ft_strcmp(section->name[i], "__const") || !ft_strcmp(section->name[i], "__objc_ivar")) && lst->tmp_type & N_EXT) 
					lst->type -= 32;
				if (!ft_strcmp(section->name[i], "__common") && (lst->tmp_type & N_PEXT || lst->tmp_type & N_EXT)) 
					lst->type = 's';
				if ((!ft_strcmp(section->name[i], "__common") || !ft_strcmp(section->name[i], "__class")  || !ft_strcmp(section->name[i], "__xcrun_shim")) && lst->tmp_type & N_EXT) 
					lst->type = 'S';
				if (!ft_strcmp(section->name[i], "__const") && lst->tmp_type & N_PEXT)
					lst->type = 's';
				if (!ft_strcmp(section->name[i], "__objc_data") && lst->tmp_type & N_EXT)
					lst->type = 'S';
				if (!ft_strcmp(lst->name, "_objc_classes") || !ft_strcmp(lst->name, "__ZL11_class_name") || !ft_strcmp(lst->name, "__non_lazy_classes"))
					lst->type = 's';
				return ;
			}
			index++;
		}
		section = section->next;
	}
}

void getType(t_func *lst, t_file *file) {
	char c;

	if (lst->type == N_UNDF) {
		c = 'U';
		lst->sect = NO_SECT;
	}
	else if ((lst->type & N_TYPE) == N_SECT) {
			get_right_section(lst, file);
			return ;
	}
	else if ((lst->type & N_TYPE) == N_PBUD)
		c = 'P';
	else
		c = 'U';
	if ((lst->type & N_TYPE) == N_INDR)
		c = 'I';
	if (((lst->type & N_TYPE) == N_ABS) && lst->type & N_EXT) {
		c = 'A';
	}
	if (lst->type & N_STAB)
		lst->name = "";
	lst->type = (char)c;
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

void addTo32(t_func **lst, char *stringtable, struct nlist table, int offset, t_file *f) {
	t_func *func;
	t_func *tmp;

	if (ft_strnstr(stringtable + (f->isSwap ? SWAP32(table.n_un.n_strx) : table.n_un.n_strx), "radr://", offset))
	 		return ;
	func = malloc(sizeof(t_func));
	func->type = f->isSwap ? SWAP32(table.n_type) : table.n_type;
	func->name = ft_strdup(stringtable + (f->isSwap ? SWAP32(table.n_un.n_strx) : table.n_un.n_strx));
	
	func->value = f->isSwap ? SWAP32(table.n_value) : table.n_value;
	if(!ft_strcmp(func->name, ""))
		func->type = N_UNDF;
	func->sect = f->isSwap ? SWAP32(table.n_sect) : table.n_sect;
	func->next = NULL;

	tmp = *lst;
	if (!tmp) {
		*lst = func;
	} else {
		find_best_place(lst, func);
	}
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
		if (ft_strlen(lst->name) > 0) {
			if ((lst->type == 'U') && f->mode == 64)
				ft_printf("                 ");
			else if ((lst->type == 'U') && f->mode == 32)
				ft_printf("         ");
			else if (f->mode == 32)
				ft_printf("%08lx ", lst->value);
			else {
				ft_printf("%016lx ", lst->value);
			}
			ft_printf("%c ", lst->type);	
			ft_printf("%s\n", lst->name);
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

	if (f->ptr + symoff > f->ptr + f->ptr_size || f->ptr + stroff > f->ptr + f->ptr_size) {
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
	stringtable = (void *)f->ptr + stroff;
	while (++i < nsyms){
		if (f->mode == 64 && f->ptr + symoff + (i * sizeof(struct nlist_64)) < f->ptr + f->ptr_size){
			addTo64(&f->lst, stringtable, array64[i], strsize, f);
		}
		else if (f->mode == 32 && f->ptr + symoff + (i * sizeof(struct nlist_64)) < f->ptr + f->ptr_size) {
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

	printf("%u\n", table.n_un.n_strx);
	printf("%d\n", offset);
	int i = -1;
	array_string = stringtable + (f->isSwap ? SWAP32(table.n_un.n_strx) : table.n_un.n_strx);
	while (++i < (f->isSwap ? SWAP32(table.n_un.n_strx) : table.n_un.n_strx) && array_string[i])
		printf("%c\n", array_string[i]);
	// if (ft_strnstr(array_string, "radr://"))
	//    	return ;
	func = malloc(sizeof(t_func));
	func->name = malloc(sizeof(char) * (i + 1));
	func->name[i] = '\0';
	// func->name = stringtable + table.n_un.n_strx, offset - (f->isSwap ? SWAP32(table.n_un.n_strx) : table.n_un.n_strx);
	ft_strncpy(func->name, stringtable + table.n_un.n_strx, i);
	//ft_memcpy(func->name, array_string, i);
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
		if (cmd && cmd->cmd && (cmd->cmd == LC_SEGMENT_64 || (f->isSwap && cmd->cmd == SWAP32(LC_SEGMENT_64)))) {
			get_sc_64((struct segment_command_64 *)((void *)f->ptr + f->lc_offset + lc_size), f);
		} else if (cmd->cmd && (cmd->cmd == LC_SEGMENT || (f->isSwap && cmd->cmd == SWAP32(LC_SEGMENT)))) {
			get_sc_32((struct segment_command *)((void *)f->ptr + f->lc_offset + lc_size), f);
		} else if (cmd->cmd && (cmd->cmd == LC_SYMTAB || (f->isSwap && cmd->cmd == SWAP32(LC_SYMTAB)))) {
			sym = (struct symtab_command *) cmd;
			print_out(f->isSwap ? SWAP32(sym->nsyms) : sym->nsyms, f->isSwap ? SWAP32(sym->symoff) : sym->symoff, f->isSwap ? SWAP32(sym->stroff) : sym->stroff, f->isSwap ? SWAP32(sym->strsize) : sym->strsize, f);
		}
		lc_size += f->isSwap ? SWAP32(cmd->cmdsize) : cmd->cmdsize;
			
	}
}


void handle_header(t_file *f) {
	uint32_t ncmds;

	if (f->is64) {
		size_t header_size = sizeof(struct mach_header_64);
		struct mach_header_64 *header = (void *)f->ptr;

		if (f->isSwap) {
		  swap_mach_header_64(header, 0);
		}
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