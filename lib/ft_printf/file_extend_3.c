/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_extend_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/20 17:02:46 by etranchi          #+#    #+#             */
/*   Updated: 2017/10/20 17:02:47 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	fill_moded_t(t_conv *cv, int f_plus, int pre, int i)
{
	int j;

	j = -1;
	if (cv->sign == '-' && f_plus == 0 && cv->empty != '0')
		cv->buffer_str[i] = cv->sign;
	else if ((f_plus == 1 && cv->empty == '0') ||
		(cv->empty == '0' && cv->sign == '-'))
		cv->buffer_str[0] = cv->sign;
	else if (f_plus == 1)
		cv->buffer_str[i - 1] = cv->sign;
	else if ((cv->empty == '0' && cv->sign == '+' && pre > 0)
		|| ft_strcmp("0", cv->buffer_nb) == 0)
		cv->buffer_str[0] = ' ';
	if (pre > (int)ft_strlen(cv->buffer_nb))
	{
		i = moded_t_pre(cv, pre, f_plus);
	}
	j = -1;
	while (cv->buffer_nb[++j])
		cv->buffer_str[++i] = cv->buffer_nb[j];
}

void	f_fill_buff(t_conv *cv, int f_plus, int pre, int space)
{
	if (cv->mode == 0)
		fill_moded_u(cv, f_plus, space);
	if (cv->mode == 2)
		fill_moded_d(cv, f_plus, pre);
	if (cv->mode == 1)
		fill_moded_t(cv, f_plus, pre, ((int)ft_strlen(cv->buffer_str)
			- (int)ft_strlen(cv->buffer_nb) - 1));
}

void	check_width(t_env *e)
{
	int i;

	i = e->buff_len - e->pre;
	while (i-- > 0)
		write(1, " ", 1);
}

int		ft_print_char(char c)
{
	write(1, &c, 1);
	return (1);
}

int		init_check(t_env *e, va_list params, int *i)
{
	if (e->fmt[*i + 1] == '\0')
		return (0);
	while (e->fmt[++(*i)] && is_flag(e->fmt, *i) == 1)
		set_flags(e->flags, e->fmt, *i);
	*i = get_buff_len(e, *i, &e->buff_len);
	if (e->fmt[*i] == '.')
	{
		*i = get_buff_len(e, ++(*i), &e->pre);
		e->flags->point = 1;
	}
	if (e->fmt[*i] && is_modifier(e->fmt, *i) == 1)
		*i = set_modifiers(e->modifiers, e->fmt, *i);
	if (is_type(e->fmt[*i]) == 1)
		e->len += set_type(e, e->fmt[(*i)++], params);
	else if (e->fmt[*i])
		e->len += set_pourcent(e, e->fmt[(*i)++]);
	else
		e->len += ft_print_char(e->fmt[(*i)++]);
	return (1);
}
