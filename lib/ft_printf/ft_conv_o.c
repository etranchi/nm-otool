/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conv_o.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Etienne <etranchi@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/22 20:43:32 by Etienne           #+#    #+#             */
/*   Updated: 2017/09/22 20:43:34 by Etienne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		modeo_d_pre(t_conv *cv, int f_plus, int pre)
{
	int i;
	int j;

	j = -1;
	i = (int)ft_strlen(cv->buffer_str) - pre;
	if (cv->sign == '-' && f_plus == 0 && cv->empty != '0')
		cv->buffer_str[i - 1] = cv->sign;
	while (pre - cv->buffer_len > 0 && (pre - ft_strlen(cv->buffer_str))
		> 0 && i < ((int)ft_strlen(cv->buffer_str) -
			(int)ft_strlen(cv->buffer_nb)))
		cv->buffer_str[i++] = '0';
	i--;
	return (i);
}

void	fill_modeo_d(t_conv *cv, int f_plus, int pre)
{
	int i;
	int j;

	j = -1;
	i = (int)ft_strlen(cv->buffer_str) - (int)ft_strlen(cv->buffer_nb) - 1;
	if (cv->sign == '-' && f_plus == 0 && cv->empty != '0')
		cv->buffer_str[i] = cv->sign;
	else if ((f_plus == 1 && cv->empty == '0') ||
		(cv->empty == '0' && cv->sign == '-'))
		cv->buffer_str[0] = cv->sign;
	else if (f_plus == 1)
		cv->buffer_str[i - 1] = cv->sign;
	else if ((cv->empty == '0' && cv->sign == '+' && pre > 0) ||
		ft_strcmp("0", cv->buffer_nb) == 0)
		cv->buffer_str[0] = ' ';
	if (pre > (int)ft_strlen(cv->buffer_nb))
	{
		i = modeo_d_pre(cv, f_plus, pre);
	}
	while (cv->buffer_nb[++j])
		cv->buffer_str[++i] = cv->buffer_nb[j];
}

void	fill_o_buffer(t_conv *cv, int f_plus, int pre)
{
	if (cv->mode == 0)
	{
		fill_modeo_u(cv);
	}
	else if (cv->mode == 2)
	{
		fill_modeo_t(cv, f_plus, pre);
	}
	else if (cv->mode == 1)
	{
		fill_modeo_d(cv, f_plus, pre);
	}
}

void	free_cv_o(t_conv *cv)
{
	free(cv->buffer_str);
	free(cv->buffer_nb);
	free(cv);
}

int		ft_conv_o(t_env *e, va_list params, char c)
{
	t_conv	*cv;
	int		len;

	cv = malloc(sizeof(t_conv));
	cv->empty = (e->flags->zero) ? '0' : ' ';
	cv->buffer_nb = check_o_modifiers(e->modifiers, params, c);
	if ((e->flags->point == 1 && e->pre == 0 &&
		ft_strcmp("0", cv->buffer_nb) == 0)
	|| cv->buffer_nb == NULL)
		cv->buffer_nb = ft_strdup("");
	create_o_buffer(cv, e);
	if (cv->buffer_nb != NULL)
	{
		if (e->flags->diez && ft_strcmp(cv->buffer_nb, "0") != 0)
			diez_o(cv);
		if (e->flags->minus == 1)
			fill_o_minus(cv, e->flags, e->pre);
		else
			fill_o_buffer(cv, e->flags->plus, e->pre);
	}
	ft_putstr(cv->buffer_str);
	len = ft_strlen(cv->buffer_str);
	free_cv_o(cv);
	return (len);
}
