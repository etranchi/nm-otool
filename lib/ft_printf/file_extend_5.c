/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_extend_5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/20 17:03:01 by etranchi          #+#    #+#             */
/*   Updated: 2017/10/20 17:03:02 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	fill_modeo_u(t_conv *cv)
{
	int i;
	int j;

	i = 0;
	j = -1;
	if (cv->sign == '-')
	{
		cv->buffer_str[0] = cv->sign;
		i++;
	}
	while (cv->buffer_nb[++j])
		cv->buffer_str[i++] = cv->buffer_nb[j];
	cv->buffer_str[i] = '\0';
}

void	fill_modeo_t(t_conv *cv, int f_plus, int pre)
{
	int i;
	int j;

	i = 0;
	j = -1;
	if (cv->sign == '-' && !f_plus && cv->empty != '0')
		cv->buffer_str[i++] = cv->sign;
	else if ((f_plus == 1 && cv->empty == '0') ||
		(cv->empty == '0' && cv->sign == '-'))
		cv->buffer_str[i++] = cv->sign;
	else if (cv->sign == '-')
	{
		cv->buffer_str = ft_strjoin("-", cv->buffer_str);
		i++;
	}
	if (pre > 0)
		while ((pre - cv->buffer_len) > 0 && ++j <
			(pre - (int)ft_strlen(cv->buffer_nb)))
			cv->buffer_str[i++] = '0';
	j = -1;
	while (cv->buffer_nb[++j])
		cv->buffer_str[i++] = cv->buffer_nb[j];
	cv->buffer_str[i] = '\0';
	if (pre == (int)ft_strlen(cv->buffer_str) && cv->sign == '-')
		cv->buffer_str = ft_strjoin("-", cv->buffer_str);
}

void	create_x_buffer(t_conv *cv, t_env *e)
{
	int len;

	len = (cv->buffer_nb) ? ft_strlen(cv->buffer_nb) : 0;
	if (e->buff_len > e->pre && e->buff_len > len)
	{
		cv->mode = 1;
		cv->buffer_str = malloc(sizeof(char) * (e->buff_len + 1));
		cv->buffer_str[e->buff_len] = '\0';
		ft_memset(cv->buffer_str, cv->empty, e->buff_len);
	}
	else if (e->pre > e->buff_len && e->pre > len)
	{
		cv->mode = 2;
		cv->buffer_str = malloc(sizeof(char) * (e->pre + 1));
		cv->buffer_str[e->pre] = '\0';
		ft_memset(cv->buffer_str, '0', e->pre);
	}
	else
	{
		cv->mode = 0;
		cv->buffer_str = malloc(sizeof(char) * (len + 1));
		cv->buffer_str[len] = '\0';
		if (len > 0)
			ft_memset(cv->buffer_str, cv->empty, len);
	}
}

void	create_bmode_u(t_conv *cv, t_env *e, int len)
{
	cv->mode = 0;
	len -= (e->pre) ? 1 : 0;
	cv->buffer_str = malloc(sizeof(char) * (len + 1));
	cv->buffer_str[len] = '\0';
	ft_memset(cv->buffer_str, cv->empty, len);
}
