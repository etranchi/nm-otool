/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 15:33:21 by etranchi          #+#    #+#             */
/*   Updated: 2019/03/28 15:33:23 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

# define ERROR 1
# define SUCCESS 0

int		ft_printf(const char *format, ...);

typedef struct			s_func {
	unsigned long		value;
	char				type;
	char				tmp_type;
	int					sect;
	char				*name;
	struct s_func		*next;
}						t_func;

typedef struct			s_section {
	char				**name;
	int					index;
	char				*segname;
	struct s_section	*next;
}						t_section;

typedef struct			s_file
{
	void				*ptr;
	int					fd;
	void				*to_give_back;
	int					header_size;
	int					ptr_size;
	int					is64;
	int					is32;
	int					is_swap;
	int					is_fat;
	int					mode;
	int					ncmds;
	int					ppc;
	int					corrupted;
	uint32_t			sizeofcmds;
	int					nm;
	char				*archive_name;
	int					offset;
	int					i;
	int					lc_offset;
	int					lst_size;
	int					nsects;
	int					buff_size;
	t_section			*section;
	struct s_func		*lst;
}						t_file;

int						error(char *error);
int						ft_printf(const char *format, ...);
int						handle_archive(t_file *file);
int						handle_fat_header(t_file *file);
int						handle_header(t_file *f);
int						handle_64(t_file *file);
int						is_magic_64(uint32_t magic);
int						should_swap_bytes(uint32_t magic);
int						is_fat(uint32_t magic);
int						get_sc_64(struct segment_command_64 *seg, t_file *file);
int						get_sc_32(struct segment_command *seg, t_file *file);
int						add_to_sections(t_section **lst, t_section *sec);
void					get_right_section(t_func *lst, t_file *file);
void					get_type(t_func *lst, t_file *file);
int						get_magic(t_file *file);
void					add_to_64(t_func **lst, char *stringtable,
	struct nlist_64 table, t_file *f);
void					add_to_32(t_func **lst, char *stringtable,
	struct nlist table, t_file *f);
void					print_otool_32(struct section *section, t_file *file);
void					print_otool_64(struct section_64 *section,
	t_file *file);
int						print_out(int nsyms, int symoff, int stroff, t_file *f);
void					print_lst(t_func *lst, t_file *f);
void					print_byte_to_hex(char byte);
void					find_best_place(t_func **lst, t_func *to_put);
int						is_magic_32(uint32_t magic);
int						is_magic_64(uint32_t magic);
int						should_swap_bytes(uint32_t magic);
int						g_s_v(int isswap, int value);
int						is_fat(uint32_t magic);
uint32_t				endian_32(uint32_t nb);
uint64_t				endian_64(uint64_t nb);
#endif
