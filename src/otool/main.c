/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 14:41:35 by etranchi          #+#    #+#             */
/*   Updated: 2019/04/10 14:41:36 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_nm.h"

int				init_file(t_file *file, char *name, int nm)
{
	int			fd;
	char		*ptr;
	struct stat	buf;

	if ((file->fd = open(name, O_RDONLY)) < 0)
		return (ERROR);
	if (fstat(file->fd, &buf) < 0)
		return (ERROR);
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, file->fd, 0))
		== MAP_FAILED)
		return (ERROR);
	file->archive_name = name;
	file->ptr = ptr;
	file->to_give_back = ptr;
	file->ptr_size = buf.st_size;
	file->lc_offset = 0;
	file->section = NULL;
	file->lst = NULL;
	file->lst_size = 0;
	file->nm = nm;
	file->error = 0;
	file->ppc = 0;
	return (SUCCESS);
}

void			give_them_back(t_file *file)
{
	t_section	*tmp;
	t_func		*tmp2;
	int			i;

	tmp = NULL;
	tmp2 = NULL;
	while (file->section)
	{
		tmp = file->section->next;
		free(file->section->segname);
		i = -1;
		while (file->section->name[++i])
			free(file->section->name[i]);
		free(file->section->name);
		free(file->section);
		file->section = tmp;
	}
	while (file->lst)
	{
		tmp2 = file->lst->next;
		free(file->lst->name);
		free(file->lst);
		file->lst = tmp2;
	}
}

void			perform_parsing(t_file *file, char **av)
{
	if (!file->nm)
		ft_printf("%s:\n", file->archive_name);
	if (get_magic(file) == ERROR)
	{
		(error("Error occured."));
		file->error = 1;
	}
	if (munmap(file->to_give_back, file->ptr_size) < 0)
	{
		(error("Error munmap."));
		file->error = 1;
	}
}

int				main(int ac, char **av)
{
	t_file		*file;
	int			i;

	i = 0;
	if (!(file = malloc(sizeof(t_file))))
		return (error("Error malloc."));
	if (ac < 2)
		return (error("No args"));
	while (++i < ac)
	{
		if (init_file(file, av[i], 0) != SUCCESS)
		{
			file->error = 1;
			error("Error about file.");
		}
		else
			perform_parsing(file, av);
	}
	give_them_back(file);
	close(file->fd);
	free(file);
	return (file->error ? ERROR : SUCCESS);
}
