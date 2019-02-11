/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hanlde_64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/11 12:38:02 by etranchi          #+#    #+#             */
/*   Updated: 2019/02/11 12:38:04 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../include/ft_nm.h"

int is_magic_64(uint32_t magic) {
  return magic == MH_MAGIC_64 || magic == MH_CIGAM_64;
}

int should_swap_bytes(uint32_t magic) {
  return magic == MH_CIGAM || magic == MH_CIGAM_64 || magic == FAT_CIGAM;
}

int is_fat(uint32_t magic) {
  return magic == FAT_MAGIC || magic == FAT_CIGAM;
}

// void print_out(int nsyms, int symoff, int stroff, char *ptr) {
// 	int i;
// 	char *stringtable;
// 	struct nlist_64 *array;
// 	void *tmp;
// 	i = -1;
// 	array = (void *)ptr + symoff;
// 	stringtable = (void *)ptr + stroff;
// 	tmp = NULL;
// 	while (++i < nsyms) {
// 		if ((int)(stringtable + array[i].n_sect) == N_UNDF) {
// 			printf("U ");
// 		} else if (((int)(stringtable + array[i].n_type) & N_TYPE) == N_ABS)  {
// 			printf("A ");
// 		} else if (((int)(stringtable + array[i].n_type) & N_TYPE) == N_SECT) {
// 			if ((int)stringtable + array[i].n_value == NO_SECT) {
// 				printf("NO SECT\n");
// 			} else {
// 				if (tmp == NULL) {
// 					tmp = (void *)(stringtable + array[i].n_value);
// 				}
// 				printf("%lx ", (void *)(stringtable + array[i].n_value) - (void *)tmp);
// 				// printf("SECT\n");
// 			}
// 			printf("T ");
// 		}
// 		else if (((int)(stringtable + array[i].n_type) & N_TYPE) == N_PBUD) {
// 			printf("P ");
// 		} else if (((int)(stringtable + array[i].n_type) & N_TYPE) == N_INDR) {
// 			printf("I " );
// 		}
// 		else {
// 			printf("U ");
// 		}
// 		// printf("%p 0x%.9X %s\n",array, &array[i] ,stringtable + array[i].n_un.n_strx);
// 		printf("%s\n", stringtable + array[i].n_un.n_strx);
// 	}

// }

// void get_file_type(t_file *file) {
// 	int type;

// 	type = *(int *)((void *)file->ptr + sizeof(uint32_t) + sizeof(cpu_type_t) + sizeof(cpu_subtype_t));
// 	file->isObject = 0;
// 	if (type == MH_OBJECT) {
// 		printf("OBJECT\n");
// 		file->isObject = 1;
// 	} else {
// 		printf("NOT OBJECT\n");
// 	}
// }


// void print_seg(struct segment_command_64 *seg, t_file *file) {
// 	int i;
// 	char stringtable;

// 	i = -1;
// 	printf("seg nsects %d\n", seg->nsects);
// 	while (++i < seg->nsects) {
// 		printf("%s\n", seg->segname);
// 	}
// }

// void handle_64(t_file *file) {
// 	int ncmds;
// 	int i;
// 	struct mach_header_64 *header;
// 	struct load_command *lc;
// 	struct symtab_command *sym;
// 	struct segment_command_64 *seg;

// 	i = -1;
// 	header = (struct mach_header_64 *)file->ptr;
// 	ncmds = header->ncmds;
// 	get_file_type(file);
// 	lc = (void *)file->ptr + sizeof(*header);
// 	while (++i < ncmds) {
// 		if (lc->cmd == LC_SEGMENT_64) {
// 			seg = (void *)lc + sizeof(struct segment_command_64);
// 			print_seg((struct segment_command_64 *)seg, file);
// 			break ;
// 		}
// 		if (lc->cmd == LC_SYMTAB) {
// 			sym = (struct symtab_command *) lc;
// 			print_out(sym->nsyms, sym->symoff, sym->stroff, file->ptr);
// 			break ;
// 		}
// 		lc = (void *)lc + lc->cmdsize;
// 	}

// }