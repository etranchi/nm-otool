

# include "../../include/ft_nm.h"



int error(void) {
	ft_printf("Error.\n");
	return (1);
}

int initFile(t_file *file, char *name, int nm) {
	int fd;
	char *ptr;
	struct stat buf;

	if ((fd = open(name, O_RDONLY)) < 0)
		return (0);
	if (fstat(fd, &buf) < 0)
		return (0);
	if ((ptr = mmap(0, buf.st_size,PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (0);
	file->archive_name = name;
	file->ptr = ptr;
	file->ptr_size = buf.st_size;
	file->lc_offset = 0;
	file->section = NULL;
	file->buff_size = buf.st_size;
	file->lst = NULL;
	file->lst_size = 0;
	file->did32 = 0;
	file->did64 = 0;
	file->nm = nm;
	file->ppc = 0;
	return (1);
}

int main(int ac, char **av) {
	
	t_file *file;
	int i = 0;

	if(!(file = malloc(sizeof(t_file))))
		return error();
	if (ac < 2) 
		return error();
	while(++i < ac) {
		if (!initFile(file, av[i], 1))
			return error();
		if(!file->nm)
			ft_printf("%s:\n", av[1]);
		get_magic(file);
		if (munmap(file->ptr, file->buff_size) < 0)
			return error();
	}
	free(file);
	return (0);
}