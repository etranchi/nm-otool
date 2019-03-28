
# include "../../include/ft_nm.h"

int main(int ac, char **av) {
	int fd;
	char *ptr;
	struct stat buf;
	t_file *file;

	if(!(file = malloc(sizeof(t_file)))) {
		ft_printf("Error malloc\n");
		return (0);
	}
	if (ac < 2) {
		ft_printf("No args..\n");
		return (0);
	}

	int i = 0;
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
	file->nm = 0;
	get_magic(file);
	if (munmap(file->to_give_back, file->ptr_size) < 0) {
		ft_printf("Error munmap\n");
		return (0);
	}
	free(file);
	return (0);
}