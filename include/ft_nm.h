
#ifndef FT_NM_H
# define FT_NM_H
# include <sys/mman.h>
# include "../lib/ft_printf/ft_printf.h"
# include <mach-o/swap.h>
# include <fcntl.h>
# include <limits.h>
# include <sys/stat.h>
# include <stdlib.h>
# include <ar.h>

#define SWAP32(x) ((((x) & 0xff000000) >> 24) | (((x) & 0xff0000) >> 8) | (((x) & 0xff00) << 8) | (((x) & 0xff) << 24))
int		ft_printf(const char *format, ...);

typedef struct s_func {
	unsigned long	value;
	char			type;
	char			tmp_type;
	char 			*indirect;
	int				sect;
	char 			*name;
	struct s_func *next;
}				t_func;


typedef struct s_section {
	char **name;
	int index;
	char *segname;
	struct s_section *next;
}				t_section;

typedef struct s_file
{
	void	*ptr;
	int		ptr_size;
	int 	is64;
	int 	is32;
	int 	isSwap;
	int 	isFat;
	int 	mode;
	int 	done;
	int		did64;
	int		did32;
	int		ncmds;
	int		ppc;
	int 	corrupted;
	uint32_t 	sizeofcmds;
	int 	nm;
	char 	*archive_name;
	int		lc_offset;
	int 	lst_size;
	int		nsects;
	int 	buff_size;
	t_section 	*section;
	struct s_func 	*lst;
}				t_file;

int		ft_printf(const char *format, ...);
void handle_64(t_file *file);
int is_magic_64(uint32_t magic);
int should_swap_bytes(uint32_t magic);
int is_fat(uint32_t magic);

void get_sc_64(struct segment_command_64 *seg, t_file *file);
void get_sc_32(struct segment_command *seg, t_file *file);
void addToSections(t_section **lst, t_section *sec);
void get_magic(t_file *file);
void handle_archive(t_file *file);
void handle_fat_header(t_file *file);
void get_magic(t_file *file);
void addTo64(t_func **lst, char *stringtable, struct nlist_64 table, int offset, t_file *f);

#endif