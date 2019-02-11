

# include "../../include/ft_nm.h"



static void dump_segment_commands(t_file *f) {
  off_t actual_offset = f->lc_offset;
  for (uint32_t i = 0; i < f->ncmds; i++) {

    struct load_command *cmd = (void *)f->ptr + actual_offset;
  
    if (f->isSwap) {
    	printf("SWAP\n");
      	swap_load_command(cmd, 0);
    }

    if (cmd->cmd == LC_SEGMENT_64) {
    	printf("ici\n");
      	struct segment_command_64 *segment = (void *)f->ptr + actual_offset;
      if (f->isSwap) {
        swap_segment_command_64(segment, 0);
      }

      printf("segname: %s\n", segment->segname);

      // free(segment);
    } else if (cmd->cmd == LC_SEGMENT) {
    	printf("la\n");
      struct segment_command *segment = (void *)f->ptr + actual_offset;
      if (f->isSwap) {
        swap_segment_command(segment, 0);
      }

      printf("segname: %s\n", segment->segname);

      // free(segment);
    }

    actual_offset += cmd->cmdsize;
    // free(cmd);
  }
}


void handle_header(t_file *f) {
	uint32_t ncmds;

	if (f->is64) {
		size_t header_size = sizeof(struct mach_header_64);
		struct mach_header_64 *header = (void *)f->ptr;
		printf("header %x\n",header->filetype & N_TYPE);
		if (f->isSwap) {
		  swap_mach_header_64(header, 0);
		}
		f->ncmds = header->ncmds;
		printf("ncmds %d\n", header->ncmds);
		f->lc_offset += header_size;
	} else {
		size_t header_size = sizeof(struct mach_header);
		struct mach_header *header = (void *)f->ptr + header_size;
		if (f->isSwap) {
		  swap_mach_header(header, 0);
		}
		f->ncmds = header->ncmds;
		f->lc_offset += header_size;
	}
	dump_segment_commands(f);
}


void get_magic(t_file *file) {
	int magic_number;

	magic_number = *(int *)file->ptr;
	file->is64 = is_magic_64(magic_number);
	file->isFat = is_fat(magic_number);
	file->isSwap = should_swap_bytes(magic_number);
	if (file->isFat) {
		// handle_fat_header(file);
	} else {
		handle_header(file);
	}
}

int main(int ac, char **av) {
	int fd;
	char *ptr;
	struct stat buf;
	t_file *file;

	if(!(file = malloc(sizeof(t_file)))) {
		printf("Error malloc\n");
		return (0);
	}
	if (ac != 2) {
		printf("No args..\n");
		return (0);
	}
	if ((fd = open(av[1], O_RDONLY)) < 0) {
		printf("Error open\n");
		return (0);
	}
	if (fstat(fd, &buf) < 0) {
		printf("Error stat\n");
		return (0);
	}
	if ((ptr = mmap(0, buf.st_size,PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		printf("Error mmap\n");
		return (0);
	}
	file->ptr = ptr;
	file->ptr_size = buf.st_size;
	file->lc_offset = 0;
	get_magic(file);
	if (munmap(file->ptr, buf.st_size) < 0) {
		printf("Error munmap\n");
		return (0);
	}

	return (0);
}