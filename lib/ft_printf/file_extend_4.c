/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_extend_4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/20 17:02:55 by etranchi          #+#    #+#             */
/*   Updated: 2017/10/20 17:02:57 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	diez_o(t_conv *cv)
{
	char *tmp;
	char *tmp1;

	tmp = ft_strdup("0");
	tmp1 = cv->buffer_nb;
	cv->buffer_nb = ft_strjoin(tmp, cv->buffer_nb);
	free(tmp);
	free(tmp1);
}

void	create_o_buffer(t_conv *cv, t_env *e)
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

void	fill_modeom_d(t_conv *cv, t_flags *flags, int pre)
{
	int i;
	int j;

	i = 0;
	j = -1;
	if (cv->sign == '-' && flags->plus == 0 && cv->empty != '0')
		cv->buffer_str[i++] = cv->sign;
	else if ((flags->plus == 1 && cv->empty == '0') || flags->plus ||
		(cv->empty == '0' && cv->sign == '-'))
		cv->buffer_str[i++] = cv->sign;
	else if ((cv->empty == '0' && cv->sign == '+' && pre > 0) ||
		ft_strcmp("0", cv->buffer_nb) == 0)
		cv->buffer_str[i++] = ' ';
	if (pre > (int)ft_strlen(cv->buffer_nb))
	{
		if (cv->sign == '-' && flags->plus == 0 && cv->empty != '0')
			cv->buffer_str[i++] = cv->sign;
		while (i < (pre - (int)ft_strlen(cv->buffer_nb)))
			cv->buffer_str[i++] = '0';
	}
	j = -1;
	while (cv->buffer_nb[++j])
		cv->buffer_str[i++] = cv->buffer_nb[j];
	while (cv->buffer_str[i])
		cv->buffer_str[i++] = ' ';
}

void	fill_modeom_t(t_conv *cv, t_flags *flags, int pre)
{
	int i;
	int j;

	i = 0;
	j = -1;
	i = pre - (int)ft_strlen(cv->buffer_nb);
	if (cv->sign == '-' && !flags->plus && cv->empty != '0')
		cv->buffer_str[i - 1] = cv->sign;
	else if ((flags->plus == 1 && cv->empty == '0') || (cv->empty == '0'
		&& cv->sign == '-'))
		cv->buffer_str[0] = cv->sign;
	j = -1;
	while (cv->buffer_nb[++j])
		cv->buffer_str[i++] = cv->buffer_nb[j];
	if (pre == (int)ft_strlen(cv->buffer_str) && cv->sign == '-')
		cv->buffer_str = ft_strjoin("-", cv->buffer_str);
}

void	fill_o_minus(t_conv *cv, t_flags *flags, int pre)
{
	int i;
	int j;

	i = 0;
	j = -1;
	if (cv->mode == 1)
	{
		fill_modeom_d(cv, flags, pre);
	}
	if (cv->mode == 2)
	{
		fill_modeom_t(cv, flags, pre);
	}
}
