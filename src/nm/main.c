

# include "../../include/ft_nm.h"

void get_magic(t_file *file);

char getType(int hash) {
	char c;

	if (hash == N_UNDF)
		c = 'U';
	else if ((hash & N_TYPE) == N_ABS)
		c = 'A';
	else if ((hash & N_TYPE) == N_SECT)
		c = 'T';
	else if ((hash & N_TYPE) == N_PBUD)
		c = 'P';
	else if ((hash & N_TYPE) == N_INDR)
		c = 'I';
	else
		c = 'U';
	return (c);
}	

void addTo(t_func **lst, char *stringtable, struct nlist_64 table) {
	t_func *func;
	t_func *tmp;

	func = malloc(sizeof(t_func));
	func->name =ft_strdup(stringtable + table.n_un.n_strx);
	func->type = getType(table.n_type);
	func->value = table.n_value;
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


void print_lst(t_func *lst) {
	while (lst) {
		printf("%lx %c %s\n", lst->value, lst->type, lst->name);
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
	print_lst(f->lst);

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
		printf("segname: %s\n", segment->segname);
    } else if (cmd->cmd == LC_SEGMENT) {
		struct segment_command *segment = (void *)f->ptr + actual_offset;
		if (f->isSwap) {
			swap_segment_command(segment, 0);
		}
		printf("segname: %s\n", segment->segname);
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
	file->lst = NULL;
	get_magic(file);
	if (munmap(file->ptr, buf.st_size) < 0) {
		printf("Error munmap\n");
		return (0);
	}

	return (0);
}