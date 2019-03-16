
# include "../../include/ft_nm.h"

#define SWAP32(x) ((((x) & 0xff000000) >> 24) | (((x) & 0xff0000) >> 8) | (((x) & 0xff00) << 8) | (((x) & 0xff) << 24))
void handle_fat_header(t_file *file) {
	struct fat_header *header = (void *)file->ptr;
	struct fat_arch *arch;
	void *tmp_ptr;
	int ppc;

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
	file->ppc = NULL;
	while (++i < (n_arch)) {
		if (SWAP32(arch->cputype) == CPU_TYPE_POWERPC)
			ppc = file->ptr + SWAP32(arch->offset);
		 if ((arch->cputype == CPU_TYPE_X86 || SWAP32(arch->cputype) == CPU_TYPE_X86_64) || (SWAP32(arch->cputype) == CPU_TYPE_I386 && i == n_arch - 1) || SWAP32(arch->cputype) == CPU_TYPE_POWERPC) {
		 	if (SWAP32(arch->cputype) == CPU_TYPE_POWERPC) {
				ppc = 1;
				ft_printf("\n%s (for architecture ppc):\n", file->archive_name);
		 	}
			if (SWAP32(arch->cputype) == CPU_TYPE_I386 && !ppc)  
				ft_printf("%s:\n", file->archive_name);
			if (SWAP32(arch->cputype) == CPU_TYPE_I386 && ppc)  
				ft_printf("\n%s (for architecture i386):\n", file->archive_name);
			file->ptr = tmp_ptr + SWAP32(arch->offset);
			get_magic(file);
			file->lc_offset = 0;
			return ;
		}
		arch++;
	}
}

