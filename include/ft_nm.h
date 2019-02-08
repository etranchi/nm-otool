
#ifndef FT_NM_H
# define FT_NM_H
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <stdlib.h>




typedef struct s_file
{
	void 	*ptr;
	int		isObject;
	int		ptr_size;
}				t_file;
#endif