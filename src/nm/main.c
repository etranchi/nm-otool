

# include "../../include/ft_nm.h"



void print_out(int nsyms, int symoff, int stroff, char *ptr) {
	int i;
	char *stringtable;
	struct nlist_64 *array;
	void *tmp;
	i = -1;
	array = (void *)ptr + symoff;
	stringtable = (void *)ptr + stroff;
	tmp = NULL;
	while (++i < nsyms) {
		if ((int)(stringtable + array[i].n_sect) == N_UNDF) {
			printf("U ");
		} else if (((int)(stringtable + array[i].n_type) & N_TYPE) == N_ABS)  {
			printf("A ");
		} else if (((int)(stringtable + array[i].n_type) & N_TYPE) == N_SECT) {
			if ((int)stringtable + array[i].n_value == NO_SECT) {
				printf("NO SECT\n");
			} else {
				if (tmp == NULL) {
					tmp = (void *)(stringtable + array[i].n_value);
				}
				printf("%lx ", (void *)(stringtable + array[i].n_value) - (void *)tmp);
				// printf("SECT\n");
			}
			printf("T ");
		}
		else if (((int)(stringtable + array[i].n_type) & N_TYPE) == N_PBUD) {
			printf("P ");
		} else if (((int)(stringtable + array[i].n_type) & N_TYPE) == N_INDR) {
			printf("I " );
		}
		else {
			printf("U ");
		}
		// printf("%p 0x%.9X %s\n",array, &array[i] ,stringtable + array[i].n_un.n_strx);
		printf("%s\n", stringtable + array[i].n_un.n_strx);
	}

}


void get_file_type(t_file *file) {
	int type;

	type = *(int *)((void *)file->ptr + sizeof(uint32_t) + sizeof(cpu_type_t) + sizeof(cpu_subtype_t));
	file->isObject = 0;
	if (type == MH_OBJECT) {
		printf("OBJECT\n");
		file->isObject = 1;
	} else {
		printf("NOT OBJECT\n");
	}
}

void handle_64(t_file *file) {
	int ncmds;
	int i;
	struct mach_header_64 *header;
	struct load_command *lc;
	struct symtab_command *sym;

	i = -1;
	header = (struct mach_header_64 *)file->ptr;
	ncmds = header->ncmds;
	get_file_type(e);
	lc = (void *)file->ptr + sizeof(*header);
	while (++i < ncmds) {
		if (lc->cmd == LC_SYMTAB) {
			sym = (struct symtab_command *) lc;
			print_out(sym->nsyms, sym->symoff, sym->stroff, file->ptr);
			break ;
		}
		lc = (void *)lc + lc->cmdsize;
	}

}


void go_nm(t_file *file) {
	int magic_number;

	magic_number = *(int *)file->ptr;
	if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
		handle_64(file);
	else if (magic_number == MH_MAGIC|| magic_number == MH_CIGAM )
		printf("32\n");
}

int main(int ac, char **av) {
	int fd;
	char *ptr;
	struct stat buf;
	t_file *file;

	e = malloc(sizeof(t_env));

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
	go_nm(file);
	if (munmap(file->ptr, buf.st_size) < 0) {
		printf("Error munmap\n");
		return (0);
	}

	return (0);
}