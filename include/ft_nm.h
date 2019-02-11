
#ifndef FT_NM_H
# define FT_NM_H
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/swap.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <stdlib.h>




typedef struct s_file
{
	void	*ptr;
	int		ptr_size;
	int 	is64;
	int 	isSwap;
	int 	isFat;
	int		ncmds;
	int		lc_offset;
}				t_file;

void handle_64(t_file *file);
int is_magic_64(uint32_t magic);
int should_swap_bytes(uint32_t magic);
int is_fat(uint32_t magic);
#endif