/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_extend_7.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/20 17:16:20 by etranchi          #+#    #+#             */
/*   Updated: 2017/10/20 17:16:21 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	create_s_buffer(t_conv *cv, t_env *e)
{
	int len;

	len = ft_strlen(cv->buffer_nb);
	if (e->buff_len > e->pre && e->buff_len > len)
	{
		cv->mode = 1;
		cv->buffer_str = malloc(sizeof(char) * (e->buff_len + 1));
		cv->buffer_str[e->buff_len] = '\0';
		ft_memset(cv->buffer_str, cv->empty, e->buff_len);
	}
	else if (e->pre > e->buff_len)
	{
		cv->mode = 2;
		cv->buffer_str = malloc(sizeof(char) * (e->pre + 1));
		cv->buffer_str[e->pre] = '\0';
		ft_memset(cv->buffer_str, cv->empty, e->pre);
		cv->buffer_len = e->buff_len;
	}
	else
		create_bmode_u(cv, e, len);
}

void	fill_s_minus(t_conv *cv, int pre)
{
	int i;
	int max;

	i = -1;
	if (cv->mode == 1)
	{
		max = (pre) ? pre : ft_strlen(cv->buffer_nb);
		while (++i < max && cv->buffer_nb[i])
			cv->buffer_str[i] = cv->buffer_nb[i];
	}
	if (cv->mode == 2)
	{
		while (cv->buffer_str[++i])
			cv->buffer_str[i] = cv->buffer_nb[i];
	}
	if (cv->mode == 0)
		ft_strcpy(cv->buffer_str, cv->buffer_nb);
}

void	fill_modes_u(t_conv *cv, int pre)
{
	int j;
	int i;

	i = -1;
	j = ft_strlen(cv->buffer_str);
	if (pre > 0)
		if (pre > (int)ft_strlen(cv->buffer_nb))
			j -= pre - (int)ft_strlen(cv->buffer_nb);
		else
			j -= pre;
	else
		j -= ft_strlen(cv->buffer_nb);
	i = -1;
	while (cv->buffer_nb[++i] && cv->buffer_str[j])
		cv->buffer_str[j++] = cv->buffer_nb[i];
}

void	fill_s_buffer(t_conv *cv, int pre)
{
	int i;
	int j;

	i = -1;
	j = 0;
	if (cv->mode == 1)
		fill_modes_u(cv, pre);
	if (cv->mode == 2)
	{
		if (cv->buffer_len > 0 &&
			cv->buffer_len > (int)ft_strlen(cv->buffer_nb))
			i = cv->buffer_len - (int)ft_strlen(cv->buffer_nb);
		else
			i = 0;
		while (cv->buffer_str[i] && cv->buffer_nb[j])
			cv->buffer_str[i++] = cv->buffer_nb[j++];
		cv->buffer_str[i] = '\0';
	}
	if (cv->mode == 0)
	{
		j = ft_strlen(cv->buffer_str);
		j -= (pre > 0) ? pre : (int)ft_strlen(cv->buffer_nb);
		while (cv->buffer_nb[++i] && cv->buffer_str[j])
			cv->buffer_str[j++] = cv->buffer_nb[i];
	}
}

void	put_little_s(t_env *e, t_conv *cv, int *len, int nul)
{
	create_s_buffer(cv, e);
	if (e->flags->minus == 1)
		fill_s_minus(cv, e->pre);
	else
		fill_s_buffer(cv, e->pre);
	if (cv->buffer_nb && ft_strcmp("(null)", cv->buffer_nb) == 0 && nul == 1)
		free(cv->buffer_nb);
	ft_putstr(cv->buffer_str);
	*len += ft_strlen(cv->buffer_str);
	free(cv->buffer_str);
}
