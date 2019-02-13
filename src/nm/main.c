

# include "../../include/ft_nm.h"

void get_magic(t_file *file);




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

	curr = (*begin);
	while (curr && curr->next != NULL)
	{
		next = curr->next;
		if (next && (ft_strcmp(curr->name, next->name) > 0))
		{
			swap_value(curr, next);
			curr = (*begin);
		}
		else
			curr = curr->next;
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
				lst->type = section->name[i][2];
				return ;
			}
			index++;
		}
		

		section = section->next;
	}
}

void getType(t_func *lst, t_file *file) {
	char c;
	char *tmp;


	if (lst->type == N_UNDF || (lst->type & N_TYPE) == N_ABS) {
		c = 'U';
		lst->sect = NO_SECT;
	}
	else if ((lst->type & N_TYPE) == N_SECT) {
		if (lst->type & N_EXT) {
			c = 'T';
		} else {
			get_right_section(lst, file);
			return ;
		}
	}
	else if ((lst->type & N_TYPE) == N_PBUD)
		c = 'P';
	else if ((lst->type & N_TYPE) == N_INDR)
		c = 'I';
	else
		c = 'U';
	lst->type = (char)c;
}	

void addTo(t_func **lst, char *stringtable, struct nlist_64 table) {
	t_func *func;
	t_func *tmp;

	func = malloc(sizeof(t_func));
	func->name = stringtable + table.n_un.n_strx;
	func->type = table.n_type;
	func->value = table.n_value;
	func->sect = table.n_sect;
	func->next = NULL;
	tmp = *lst;
	if (!tmp) {
		*lst = func;
		return;
	} else {
		while (tmp && tmp->next)
			tmp = tmp->next;
		tmp->next = func;
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
		printf("0000000%lx ", lst->value);
		printf("                 ");
		printf("%c ", lst->type);
		printf("%s\n", lst->name);
		lst = lst->next;
	}
}

void print_out(int nsyms, int symoff, int stroff, t_file *f) {
	int i;
	char *stringtable;
	struct nlist_64 *array;

	i = -1;
	array = (void *)f->ptr + symoff;
	stringtable = (void *)f->ptr + stroff;
	while (++i < nsyms) {
		// printf("i %d\n", i);
		// if ((int)(stringtable + array[i].n_sect) == N_UNDF) {
		// 	printf("U ");
		// } else if (((int)(stringtable + array[i].n_type) & N_TYPE) == N_ABS)  {
		// 	printf("A ");
		// } else if (((int)(stringtable + array[i].n_type) & N_TYPE) == N_SECT) {
		// 	if ((int)stringtable + array[i].n_value == NO_SECT) {
		// 		printf("NO SECT\n");
		// 	} else {
		// 		printf("00000000%lx ", (void *)(array[i].n_value));
		// 	}
		// 	printf("T ");
		// }
		// else if (((int)(stringtable + array[i].n_type) & N_TYPE) == N_PBUD) {
		// 	printf("P ");
		// } else if (((int)(stringtable + array[i].n_type) & N_TYPE) == N_INDR) {
		// 	printf("I " );
		// }
		// else {
		// 	printf("                  U ");
		// }
		// printf("%p 0x%.9X %s\n",array, &array[i] ,stringtable + array[i].n_un.n_strx);

		addTo(&f->lst, stringtable, array[i]);
	}
	sort_name(&f->lst);

	print_lst(f->lst, f);

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
	sec->name[seg->nsects] = '\0';
	sec->index = index++;
	section = (struct section_64*)(seg + 1);
	sec->next = NULL;
	i = -1;
	while (++i < (int)seg->nsects) {
		if (!(sec->name[i] = ft_strdup(section->sectname))) {
			return;
		}
		section++;
	}
	addToSections(&file->section, sec);
	return ;
}

static void dump_segment_commands(t_file *f) {
  off_t actual_offset = f->lc_offset;
  for (uint32_t i = 0; i < f->ncmds; i++) {
    struct load_command *cmd = (void *)f->ptr + actual_offset;
    if (f->isSwap) {
      	swap_load_command(cmd, 0);
    }
    if (cmd->cmd == LC_SEGMENT_64) {
		struct segment_command_64 *segment = (void *)f->ptr + actual_offset;
		if (f->isSwap) {
			swap_segment_command_64(segment, 0);
		}
		get_sc_64(segment, f);
    } else if (cmd->cmd == LC_SEGMENT) {
		struct segment_command *segment = (void *)f->ptr + actual_offset;
		if (f->isSwap) {
			swap_segment_command(segment, 0);
		}
    } else if (cmd->cmd == LC_SYMTAB) {
    	struct symtab_command *sym = (struct symtab_command *) cmd;
    	print_out(sym->nsyms, sym->symoff, sym->stroff, f);
    }
    actual_offset += cmd->cmdsize;
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
		f->ncmds = header->ncmds;
		f->lc_offset += header_size;
	} else {
		size_t header_size = sizeof(struct mach_header);
		struct mach_header *header = (void *)f->ptr + header_size;
		if (f->isSwap) {
		  swap_mach_header(header, 0);
		}
		f->ncmds = header->ncmds;
		f->lc_offset += header_size;
	}
	dump_segment_commands(f);
}


void get_magic(t_file *file) {
	int magic_number;

	magic_number = *(int *)file->ptr;
	file->is64 = is_magic_64(magic_number);
	file->isFat = is_fat(magic_number);
	file->isSwap = should_swap_bytes(magic_number);
	if (file->isFat) {
		// handle_fat_header(file);
	} else {
		handle_header(file);
	}
}

int main(int ac, char **av) {
	int fd;
	char *ptr;
	struct stat buf;
	t_file *file;

	if(!(file = malloc(sizeof(t_file)))) {
		printf("Error malloc\n");
		return (0);
	}
	if (ac != 2) {
		printf("No args..\n");
		return (0);
	}
	if ((fd = open(av[1], O_RDONLY)) < 0) {
		printf("Error open\n");
		return (0);
	}
	if (fstat(fd, &buf) < 0) {
		printf("Error stat\n");
		return (0);
	}
	if ((ptr = mmap(0, buf.st_size,PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		printf("Error mmap\n");
		return (0);
	}
	file->ptr = ptr;
	file->ptr_size = buf.st_size;
	file->lc_offset = 0;
	file->section = NULL;
	file->lst = NULL;
	file->lst_size = 0;
	get_magic(file);
	if (munmap(file->ptr, buf.st_size) < 0) {
		printf("Error munmap\n");
		return (0);
	}
	free(file);
	return (0);
}