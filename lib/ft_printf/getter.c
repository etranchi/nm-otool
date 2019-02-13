/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 15:00:12 by etranchi          #+#    #+#             */
/*   Updated: 2017/09/25 15:29:22 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		get_precision(t_env *e, int i, int *len)
{
	char	buff[10];
	int		j;

	j = -1;
	ft_memset(buff, ' ', 3);
	while (ft_isdigit(e->fmt[i]))
		buff[++j] = e->fmt[i++];
	*len = ft_atoi(buff);
	while (ft_isdigit(e->fmt[i]))
		i++;
	return (i);
}

int		get_buff_len(t_env *e, int i, int *len)
{
	char	*buffer;
	int		j;

	j = -1;
	buffer = malloc(sizeof(char) * 4);
	buffer[3] = '\0';
	while (ft_isdigit(e->fmt[i]))
		buffer[++j] = e->fmt[i++];
	buffer[++j] = '\0';
	*len = ft_atoi(buffer);
	free(buffer);
	return (i);
}

void	init_flags_modi(t_flags *flags, t_modifiers *modifiers)
{
	flags->diez = 0;
	flags->minus = 0;
	flags->plus = 0;
	flags->space = 0;
	flags->zero = 0;
	flags->point = 0;
	modifiers->h = 0;
	modifiers->l = 0;
	modifiers->hh = 0;
	modifiers->ll = 0;
	modifiers->j = 0;
	modifiers->z = 0;
}
