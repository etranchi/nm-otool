/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conv_u.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/06 17:38:49 by etranchi          #+#    #+#             */
/*   Updated: 2017/09/06 17:38:50 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	fill_modeu_d(t_conv *cv, int pre, int minus)
{
	int i;
	int j;

	j = -1;
	i = (int)ft_strlen(cv->buffer_str) - (int)ft_strlen(cv->buffer_nb) - 1;
	if (minus == 1)
		i = -1;
	if (pre > (int)ft_strlen(cv->buffer_nb))
	{
		i -= (pre - (int)ft_strlen(cv->buffer_nb));
		while ((pre - cv->buffer_len) > 0 && ++j < pre - cv->buffer_len)
			cv->buffer_str[++i] = '0';
	}
	j = -1;
	while (cv->buffer_nb[++j])
		cv->buffer_str[++i] = cv->buffer_nb[j];
}

void	fill_u(t_conv *cv, int pre, int minus)
{
	int i;
	int j;

	i = -1;
	j = -1;
	if (cv->mode == 0)
		while (cv->buffer_nb[++j])
			cv->buffer_str[++i] = cv->buffer_nb[j];
	if (cv->mode == 1)
		fill_modeu_d(cv, pre, minus);
	if (cv->mode == 2)
	{
		i = 0;
		if (pre > 0)
			while ((pre - cv->buffer_len) > 0 && ++j < (pre - cv->buffer_len))
				cv->buffer_str[i++] = '0';
		j = -1;
		while (cv->buffer_nb[++j])
			cv->buffer_str[i++] = cv->buffer_nb[j];
	}
}

int		ft_conv_u(t_env *e, va_list params, char c)
{
	t_conv	*cv;
	int		len;
	char	*tmp;

	cv = malloc(sizeof(t_conv));
	cv->empty = (e->flags->zero) ? '0' : ' ';
	cv->buffer_nb = check_u_modifiers(e->modifiers, params, c);
	if (e->flags->point == 1 && e->pre == 0 &&
		ft_strcmp("0", cv->buffer_nb) == 0)
	{
		tmp = cv->buffer_nb;
		cv->buffer_nb = ft_strdup("");
		free(tmp);
	}
	cv->buffer_len = (int)ft_strlen(cv->buffer_nb);
	create_d_buffer(cv, e);
	fill_u(cv, e->pre, e->flags->minus);
	ft_putstr(cv->buffer_str);
	len = ft_strlen(cv->buffer_str);
	free(cv->buffer_nb);
	free(cv->buffer_str);
	free(cv);
	return (len);
}
