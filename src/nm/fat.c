
# include "../../include/ft_nm.h"



void handle_fat_header(t_file *file) {
	struct fat_header *header = (void *)file->ptr;
	struct fat_arch *arch;
	void *tmp_ptr;

	int n_arch;
	int i;
	tmp_ptr = file->ptr;
	if (file->isSwap) {
		n_arch = SWAP32(header->nfat_arch);
	} else {
		n_arch = header->nfat_arch;
	}
	arch = (void *)header + sizeof(*header);
	i = -1;
	file->ppc = 0;
	file->corrupted = 0;
	while (++i < (n_arch) && !file->corrupted) {
		if (n_arch > 1 && file->ppc)
			ft_printf("\n");
		if ((file->isSwap ? SWAP32(arch->offset) : arch->offset) + (file->isSwap ? SWAP32(arch->size) : arch->size) > file->buff_size) {
			ft_printf("Corrupted\n");
			return ;
		}
		if ((arch->cputype == CPU_TYPE_X86 || SWAP32(arch->cputype) == CPU_TYPE_X86_64) || (SWAP32(arch->cputype) == CPU_TYPE_I386 && i == n_arch - 1) || SWAP32(arch->cputype) == CPU_TYPE_POWERPC) {
		 	if (SWAP32(arch->cputype) == CPU_TYPE_POWERPC) {
				file->ppc = 1;
				ft_printf("\n%s", file->archive_name);
				if (n_arch > 1)
					ft_printf(" (for architecture ppc):\n");
				else 
					ft_printf(":\n");
		 	}
			if ((SWAP32(arch->cputype) == CPU_TYPE_I386 || arch->cputype == CPU_TYPE_I386) && file->ppc)
			{
				ft_printf("%s", file->archive_name);
				if (n_arch > 1)
					ft_printf(" (for architecture i386):\n");
				else 
					ft_printf(":\n");
			}
			
			file->ptr = tmp_ptr + SWAP32(arch->offset);
			get_magic(file);
			file->lc_offset = 0;
			if (!file->ppc)
				return ;
		}
		arch++;
	}
}

