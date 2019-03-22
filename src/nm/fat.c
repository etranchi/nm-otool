
# include "../../include/ft_nm.h"



void handle_fat_header(t_file *file) {
	struct fat_header *header = (void *)file->ptr;
	struct fat_arch *arch;
	void *tmp_ptr;
	int n_arch;
	int i;
	tmp_ptr = file->ptr;

	n_arch = file->isSwap ? SWAP32(header->nfat_arch) : header->nfat_arch;
	arch = (void *)header + sizeof(*header);
	i = -1;
	file->ppc = 0;
	file->corrupted = 0;
	while (++i < (n_arch) && !file->corrupted) {
		if (n_arch > 1 && file->ppc)
			ft_printf("\n");
		if ((file->isSwap ? SWAP32(arch->cputype) : arch->cputype) == CPU_TYPE_POWERPC) {
			file->ppc = 1;
			ft_printf("Don't handle ppc, sorry..\n");
			return ;
			if (n_arch > 1)
				ft_printf(" (for architecture ppc):\n");
			else 
				ft_printf(":\n");
	 	}
	 	if (file->ppc && ((file->isSwap ? SWAP32(arch->cputype) : arch->cputype) == CPU_TYPE_X86_64 || (file->isSwap ? SWAP32(arch->cputype) : arch->cputype) == CPU_TYPE_X86)) {
	 		if (file->nm)
				ft_printf("%s", file->archive_name);
			if (n_arch > 1) {
				if ((file->isSwap ? SWAP32(arch->cputype) : arch->cputype) == CPU_TYPE_X86)
					ft_printf(" (for architecture i386):\n");
				else
					ft_printf(" (for architecture x86_64):\n");
			}
			else 
				ft_printf(":\n");
		}
		if ((!file->ppc && (file->isSwap ? SWAP32(arch->cputype) : arch->cputype) == CPU_TYPE_X86) && n_arch > 1)
			arch++;
		else {
			file->ptr = tmp_ptr + (file->isSwap ? SWAP32(arch->offset) : arch->offset);
			file->lc_offset = 0;
			file->header_size = (file->isSwap ? SWAP32(arch->size) : arch->size);
			get_magic(file);
			if (!file->ppc)
				return ;	
		}
	}
}

