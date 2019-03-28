
# include "../../include/ft_nm.h"

int handle_archive(t_file *file) {
	struct ar_hdr *header;
	uint32_t offset;
	void *tmp_ptr;
	struct ar_hdr *header_to_nm;
	char **tmp;

	header = (struct ar_hdr *)(file->ptr + SARMAG);
	offset = ft_atoi(header->ar_size) + sizeof(struct ar_hdr);

	header = (struct ar_hdr *) ((void *) header + offset);
	ft_printf("Archive : %s", file->archive_name);
	while (42) {
		tmp_ptr = file->ptr;
		tmp = ft_strsplit(header->ar_name, '\n');
		if (!ft_strcmp(header->ar_name, "") || tmp[1][0] == '#')
			return (ERROR);
		ft_printf("\n%s(%s):\n", file->archive_name, tmp[1]);	
		header_to_nm = (struct ar_hdr *)((void *)header + sizeof(struct ar_hdr));
		tmp = ft_strsplit(tmp[0], '/');
		header_to_nm = (void *)header_to_nm + ft_atoi(tmp[1]);
		file->ptr =(void *)header_to_nm;
		get_magic(file);
		offset = ft_atoi(header->ar_size) + sizeof(struct ar_hdr);
		if (((void *)header_to_nm - tmp_ptr + offset + ft_atoi(header_to_nm->ar_size)) > (file->ptr_size)) {
			return (ERROR);
		}
		header = ((void *)header + offset);
	}
	file->ptr = tmp_ptr;
	return (SUCCESS);
}