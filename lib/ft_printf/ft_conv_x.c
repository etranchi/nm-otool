/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conv_x.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Etienne <etranchi@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/17 18:39:39 by Etienne           #+#    #+#             */
/*   Updated: 2017/09/17 18:39:40 by Etienne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	fill_x_buffer(t_conv *cv, int diez, int pre, char c)
{
	int		i;
	int		j;
	char	*tmp;
	char	*tmp1;

	i = -1;
	j = -1;
	tmp = (c == 'x') ? ft_strdup("0x") : ft_strdup("0X");
	if (cv->mode == 1)
		fill_modex_d(cv, diez, pre, tmp);
	if (cv->mode == 2)
		fill_modex_t(cv, diez, tmp);
	if (cv->mode == 0)
	{
		while (cv->buffer_nb[++i] != '\0')
			cv->buffer_str[i] = cv->buffer_nb[i];
		if (diez == 1 && ft_strcmp("0", cv->buffer_nb) != 0)
		{
			tmp1 = cv->buffer_str;
			cv->buffer_str = ft_strjoin(tmp, cv->buffer_str);
			free(tmp1);
		}
	}
	free(tmp);
}

void	fill_modexm_d(t_conv *cv, int diez, char *tmp)
{
	int i;
	int j;

	i = -1;
	j = 0;
	if (diez == 1 && ft_strcmp("0", cv->buffer_nb) != 0)
	{
		cv->buffer_str[j++] = tmp[0];
		cv->buffer_str[j++] = tmp[1];
	}
	while (cv->buffer_nb[++i])
		cv->buffer_str[j++] = cv->buffer_nb[i];
	while (cv->buffer_str[j])
		cv->buffer_str[j++] = ' ';
}

void	fill_modexm_t(t_conv *cv, int diez, char *tmp)
{
	char	*tmp1;
	int		i;
	int		j;

	i = -1;
	j = 0;
	while (cv->buffer_nb[++i])
		cv->buffer_str[j++] = cv->buffer_nb[i];
	if (diez == 1 && ft_strcmp("0", cv->buffer_nb) != 0)
	{
		tmp1 = cv->buffer_str;
		cv->buffer_str = ft_strjoin(tmp, cv->buffer_str);
		free(tmp1);
	}
}

void	fill_x_minus_buffer(t_conv *cv, int diez, char c)
{
	int		i;
	int		j;
	char	*tmp;
	char	*tmp1;

	i = -1;
	j = -1;
	tmp = (c == 'x') ? ft_strdup("0x") : ft_strdup("0X");
	if (cv->mode == 1)
		fill_modexm_d(cv, diez, tmp);
	if (cv->mode == 2)
		fill_modexm_t(cv, diez, tmp);
	if (cv->mode == 0)
	{
		while (cv->buffer_nb[++i] != '\0')
			cv->buffer_str[i] = cv->buffer_nb[i];
		if (diez == 1 && ft_strcmp("0", cv->buffer_nb) != 0)
		{
			tmp1 = cv->buffer_str;
			cv->buffer_str = ft_strjoin(tmp, cv->buffer_str);
			free(tmp1);
		}
	}
	free(tmp);
}

int		ft_conv_x(t_env *e, va_list params, char c)
{
	t_conv	*cv;
	int		len;

	cv = malloc(sizeof(t_conv));
	cv->empty = (e->flags->zero) ? '0' : ' ';
	cv->buffer_nb = check_x_modifiers(e->modifiers, params, c);
	if (e->flags->point == 1 && e->pre == 0)
		cv->buffer_nb = NULL;
	create_x_buffer(cv, e);
	if (cv->buffer_nb != NULL)
	{
		if (e->flags->minus == 1)
			fill_x_minus_buffer(cv, e->flags->diez, c);
		else
			fill_x_buffer(cv, e->flags->diez, e->pre, c);
	}
	ft_putstr(cv->buffer_str);
	len = (int)ft_strlen(cv->buffer_str);
	free(cv->buffer_str);
	free(cv->buffer_nb);
	free(cv);
	return (len);
}
