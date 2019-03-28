

# include "../../include/ft_nm.h"

int initFile(t_file *file, char *name, int nm) {
	int fd;
	char *ptr;
	struct stat buf;

	if ((fd = open(name, O_RDONLY)) < 0)
		return (error("Error when opening file."));
	if (fstat(fd, &buf) < 0)
		return (error("Error getting information about this file."));
	if ((ptr = mmap(0, buf.st_size,PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (error("Error mmap."));
	file->archive_name = name;
	file->ptr = ptr;
	file->to_give_back = ptr;
	file->ptr_size = buf.st_size;
	file->lc_offset = 0;
	file->section = NULL;
	file->lst = NULL;
	file->lst_size = 0;
	file->nm = nm;
	file->ppc = 0;
	return (SUCCESS);
}

int main(int ac, char **av) {
	
	t_file *file;
	int i;

	i = 0;
	if(!(file = malloc(sizeof(t_file))))
		return (error("Error malloc."));
	if (ac < 2) 
		return (error("No args"));
	while(++i < ac) {
		if (initFile(file, av[i], 1) != SUCCESS)
			return (ERROR);
		if(!file->nm)
			ft_printf("%s:\n", av[1]);
		if (get_magic(file) == ERROR) 
			return (ERROR);
		if (munmap(file->to_give_back, file->ptr_size) < 0)
			return (error("Error munmap."));
	}
	free(file);
	return (SUCCESS);
}