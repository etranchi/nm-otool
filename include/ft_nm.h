
#ifndef FT_NM_H
# define FT_NM_H
# include <sys/mman.h>
# include "../lib/ft_printf/ft_printf.h"
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/swap.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <stdlib.h>


int		ft_printf(const char *format, ...);

typedef struct s_func {
	unsigned long	value;
	char			type;
	int				sect;
	char 			*name;
	struct s_func *next;
}				t_func;


typedef struct s_section {
	char **name;
	int index;
	struct s_section *next;
}				t_section;

typedef struct s_file
{
	void	*ptr;
	int		ptr_size;
	int 	is64;
	int 	isSwap;
	int 	isFat;
	int		ncmds;
	int		lc_offset;
	int 	lst_size;
	int		nsects;
	t_section 	*section;
	struct s_func 	*lst;
}				t_file;

void handle_64(t_file *file);
int is_magic_64(uint32_t magic);
int should_swap_bytes(uint32_t magic);
int is_fat(uint32_t magic);
#endif