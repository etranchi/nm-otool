

# include "../../include/ft_nm.h"


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
				if (!ft_strcmp(section->name[i], "__data"))
					lst->type = 'd';
				if (!ft_strcmp(section->name[i], "__data") && (lst->tmp_type & N_EXT))
					lst->type -= 32;
				if (!ft_strcmp(section->name[i], "__thread_bss") || !ft_strcmp(section->name[i], "__thread_vars") || !ft_strcmp(section->name[i], "__common") || !ft_strcmp(section->name[i], "__objc_ivar") || !ft_strcmp(section->name[i], "__program_vars") || !ft_strcmp(section->name[i], "__eh_frame") || !ft_strcmp(section->name[i], "__objc_data") || !ft_strcmp(section->name[i], "__gcc_except_tab__TEXT") || !ft_strcmp(section->name[i], "__cstring") || !ft_strcmp(section->name[i], "__crash_info") || !ft_strcmp(section->name[i], "__const") || !ft_strcmp(section->name[i], "__ustring") || !ft_strcmp(section->name[i], "__info_plist") )
					lst->type = 's';
				if (!ft_strcmp(section->name[i], "__thread_vars") && lst->tmp_type & N_PEXT)
					lst->type = 't';
				if ((!ft_strcmp(section->name[i], "__const") || !ft_strcmp(section->name[i], "__objc_ivar")) && lst->tmp_type & N_EXT) 
					lst->type -= 32;
				if (!ft_strcmp(section->name[i], "__common") && (lst->tmp_type & N_PEXT || lst->tmp_type & N_EXT)) 
					lst->type = 's';
				if ((!ft_strcmp(section->name[i], "__common") || !ft_strcmp(section->name[i], "__class")  || !ft_strcmp(section->name[i], "__xcrun_shim")) && lst->tmp_type & N_EXT) 
					lst->type = 'S';
				if (!ft_strcmp(section->name[i], "__objc_data"))
					lst->type = 'S';
				if (!ft_strcmp(section->name[i], "__const") && lst->tmp_type & N_PEXT)
					lst->type = 's';
				if (!ft_strcmp(section->name[i], "__objc_data") && lst->tmp_type & N_PEXT)
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

	if (lst->type == N_UNDF || (lst->type & N_TYPE) == N_ABS) {
		c = 'U';
		lst->sect = NO_SECT;
	}
	else if ((lst->type & N_TYPE) == N_SECT) {
			get_right_section(lst, file);
			return ;
	}
	else if ((lst->type & N_TYPE) == N_PBUD)
		c = 'P';
	else if ((lst->type & N_TYPE) == N_INDR)
		c = 'I';
	else
		c = 'U';
	if (lst->type & N_ABS && lst->type & N_EXT)
		c = 'A';
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
		// if ( ft_strcmp(tmp->name, "_exchange_options") == 0 && ft_strcmp(to_put->name, "_exchange_options") == 0 ) {
		// 	printf("tmp %s toput %s\n", tmp->value, to_put->value);
		// }
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

void addTo32(t_func **lst, char *stringtable, struct nlist table) {
	t_func *func;
	t_func *tmp;

	
	if (ft_strstr((stringtable + table.n_un.n_strx), "radr://"))
			return ;
	func = malloc(sizeof(t_func));
	func->name = ft_strdup(stringtable + table.n_un.n_strx);
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

void addTo64(t_func **lst, char *stringtable, struct nlist_64 table) {
	t_func *func;
	t_func *tmp;

	
	if (ft_strstr((stringtable + table.n_un.n_strx), "radr://"))
			return ;
	func = malloc(sizeof(t_func));
	func->name = stringtable + table.n_un.n_strx;
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
			if (lst->type == 'U' && f->mode == 64)
				ft_printf("                 ");
			else if (lst->type == 'U' && f->mode == 32)
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

void print_out(int nsyms, int symoff, int stroff, t_file *f) {
	int i;
	char *stringtable;
	struct nlist_64 *array64;
	struct nlist 	*array32;
	i = -1;

	if (f->mode == 64) 
		array64 = (void *)f->ptr + symoff;
	else 
		array32 = (void *)f->ptr + symoff;
	stringtable = (void *)f->ptr + stroff;
	while (++i < nsyms) {
		f->mode == 64 ? addTo64(&f->lst, stringtable, array64[i]) :  addTo32(&f->lst, stringtable, array32[i]);
	}
	// sort_name(&f->lst);
	if (!f->isFat)
		print_lst(f->lst, f);
}




static void dump_segment_commands(t_file *f) {
	int i;
	struct load_command *cmd;
	struct symtab_command *sym;

	i = -1;
	while(++i < f->ncmds) {
		cmd = (void *)f->ptr + f->lc_offset;
		if (f->isSwap) 
		  	swap_load_command(cmd, 0);
		if (cmd->cmd == LC_SEGMENT_64) {
			if (f->isSwap)
				swap_segment_command_64((struct segment_command_64 *)((void *)f->ptr + f->lc_offset), 0);
			get_sc_64((struct segment_command_64 *)((void *)f->ptr + f->lc_offset), f);
		} else if (cmd->cmd == LC_SEGMENT) {
			if (f->isSwap)
				swap_segment_command((struct segment_command *)((void *)f->ptr + f->lc_offset), 0);
			get_sc_32((struct segment_command *)((void *)f->ptr + f->lc_offset), f);
		} else if (cmd->cmd == LC_SYMTAB) {
			sym = (struct symtab_command *) cmd;
			print_out(sym->nsyms, sym->symoff, sym->stroff, f);
		}
		f->lc_offset += cmd->cmdsize;
	}
}


void handle_header(t_file *f) {
	uint32_t ncmds;

	if (f->is64) {
		// printf("64\n");
		size_t header_size = sizeof(struct mach_header_64);
		struct mach_header_64 *header = (void *)f->ptr;
		if (f->isSwap) {
		  swap_mach_header_64(header, 0);
		}
		f->mode = 64;
		f->ncmds = header->ncmds;
		f->lc_offset += header_size;
	} else {
		size_t header_size = sizeof(struct mach_header);
		struct mach_header *header = (void *)f->ptr;
		if (f->isSwap) {
		  swap_mach_header(header, 0);
		}
		f->mode = 32;
		f->ncmds = header->ncmds;
		f->lc_offset += header_size;

	}
	dump_segment_commands(f);
}


void handle_fat_header(t_file *file) {
	struct fat_header *header = (void *)file->ptr;
	struct fat_arch *arch;
	void *tmp_ptr;

	int n_arch;
	int i;
	tmp_ptr = file->ptr;
	if (file->isSwap) {
		n_arch = SWAP32(header->nfat_arch);
	} else {
		n_arch = header->nfat_arch;
	}
	
	arch = (void *)header + sizeof(*header);
	i = -1;
	while (++i < (n_arch)) {
		if ((arch->cputype == CPU_TYPE_X86 || SWAP32(arch->cputype) == CPU_TYPE_X86_64)) {
			file->ptr = tmp_ptr + SWAP32(arch->offset);
			get_magic(file);
			file->lc_offset = 0;
			return ;
		}
		arch++;
	}
}

void handle_archive(t_file *file) {
	struct ar_hdr *header;
	uint32_t offset;
	void *tmp_ptr;

	header = (struct ar_hdr *)(file->ptr + SARMAG);
	offset = ft_atoi(header->ar_size) + sizeof(struct ar_hdr) + SARMAG + (ft_strlen(ARFMAG)*sizeof(char));
	printf("%d\n", offset);
	header = (struct ar_hdr *) ((void *) header + offset);
	tmp_ptr = file->ptr;
	while(42) {
		char **tmp;
		tmp = ft_strsplit(header->ar_name, '\n');
		if (!ft_strcmp(header->ar_name, ""))
			break ;
		printf("\n%s: (%s)\n", file->archive_name, tmp[1]);	
		tmp = ft_strsplit(tmp[0], ' ');
		offset += sizeof(struct ar_hdr) + ft_atoi(header->ar_size);
		file->ptr = (void *)tmp_ptr + offset;
		printf("offset %lx\n", offset);
		get_magic(file);
		header = (struct ar_hdr *) ((void *) header + offset);
	}
	
}

void get_magic(t_file *file) {
	int magic_number;

	magic_number = *(int *)file->ptr;
	file->is64 = is_magic_64(magic_number);
	file->isFat = is_fat(magic_number);
	file->isSwap = should_swap_bytes(magic_number);
	if (file->isFat) {
		handle_fat_header(file);
	} else if (ft_strncmp(file->ptr, ARMAG, SARMAG) == 0) {
		handle_archive(file);
	}
	else {
		handle_header(file);
	}
}

int main(int ac, char **av) {
	int fd;
	char *ptr;
	struct stat buf;
	t_file *file;

	if(!(file = malloc(sizeof(t_file)))) {
		ft_printf("Error malloc\n");
		return (0);
	}
	if (ac != 2) {
		ft_printf("No args..\n");
		return (0);
	}
	if ((fd = open(av[1], O_RDONLY)) < 0) {

		ft_printf("Error open\n");
		return (0);
	}
	if (fstat(fd, &buf) < 0) {
		ft_printf("Error stat\n");
		return (0);
	}
	if ((ptr = mmap(0, buf.st_size,PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		ft_printf("Error mmap\n");
		return (0);
	}
	file->archive_name = av[1];
	file->ptr = ptr;
	file->ptr_size = buf.st_size;
	file->lc_offset = 0;
	file->section = NULL;
	file->lst = NULL;
	file->lst_size = 0;
	file->did32 = 0;
	file->did64 = 0;
	get_magic(file);
	if (munmap(file->ptr, buf.st_size) < 0) {
		ft_printf("Error munmap\n");
		return (0);
	}
	free(file);
	return (0);
}