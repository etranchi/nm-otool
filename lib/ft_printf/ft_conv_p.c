/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conv_p.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Etienne <etranchi@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/19 14:50:29 by Etienne           #+#    #+#             */
/*   Updated: 2017/09/19 14:50:31 by Etienne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	fill_modep_d(t_conv *cv, char *tmp, int f_zero)
{
	int j;
	int i;

	i = -1;
	j = (int)ft_strlen(cv->buffer_str) - (int)ft_strlen(cv->buffer_nb);
	if (j > 1 && f_zero)
	{
		cv->buffer_str[0] = tmp[0];
		cv->buffer_str[1] = tmp[1];
	}
	else if (j > 1 && !f_zero)
	{
		cv->buffer_str[j - 2] = tmp[0];
		cv->buffer_str[j - 1] = tmp[1];
	}
	while (cv->buffer_nb[++i])
		cv->buffer_str[j++] = cv->buffer_nb[i];
}

void	fill_p_buffer(t_conv *cv, int f_zero)
{
	int		i;
	int		j;
	char	*tmp;
	char	*tmp1;

	i = -1;
	j = (int)ft_strlen(cv->buffer_str) - (int)ft_strlen(cv->buffer_nb);
	tmp = ft_strdup("0x");
	if (cv->mode == 1)
		fill_modep_d(cv, tmp, f_zero);
	if (cv->mode == 2)
	{
		while (cv->buffer_nb[++i])
			cv->buffer_str[j++] = cv->buffer_nb[i];
		cv->buffer_str = ft_strjoin(tmp, cv->buffer_str);
	}
	if (cv->mode == 0)
	{
		while (cv->buffer_nb[++i] != '\0')
			cv->buffer_str[i] = cv->buffer_nb[i];
		tmp1 = cv->buffer_str;
		cv->buffer_str = ft_strjoin(tmp, cv->buffer_str);
		free(tmp1);
	}
	free(tmp);
}

void	fill_modepm_d(t_conv *cv, char *tmp)
{
	int j;
	int i;

	j = 0;
	i = -1;
	cv->buffer_nb = ft_strjoin(tmp, cv->buffer_nb);
	while (cv->buffer_nb[++i])
		cv->buffer_str[j++] = cv->buffer_nb[i];
}

void	fill_p_minus_buffer(t_conv *cv)
{
	int		i;
	int		j;
	char	*tmp;
	char	*tmp1;

	i = -1;
	j = (int)ft_strlen(cv->buffer_str) - (int)ft_strlen(cv->buffer_nb);
	tmp = ft_strdup("0x");
	if (cv->mode == 1)
		fill_modepm_d(cv, tmp);
	if (cv->mode == 2)
	{
		while (cv->buffer_nb[++i])
			cv->buffer_str[j++] = cv->buffer_nb[i];
		cv->buffer_str = ft_strjoin(tmp, cv->buffer_str);
	}
	if (cv->mode == 0)
	{
		while (cv->buffer_nb[++i] != '\0')
			cv->buffer_str[i] = cv->buffer_nb[i];
		tmp1 = cv->buffer_str;
		cv->buffer_str = ft_strjoin(tmp, cv->buffer_str);
		free(tmp1);
	}
	free(tmp);
}

int		ft_conv_p(t_env *e, va_list params)
{
	t_conv	*cv;
	int		len;

	cv = malloc(sizeof(t_conv));
	cv->empty = (e->flags->zero == 1) ? '0' : ' ';
	cv->buffer_nb = check_p_modifiers(e->modifiers, params);
	if ((e->flags->point == 1 && e->pre == 0) || cv->buffer_nb == NULL)
		cv->buffer_nb = ft_strdup("");
	create_d_buffer(cv, e);
	if (e->flags->minus == 1)
		fill_p_minus_buffer(cv);
	else
		fill_p_buffer(cv, e->flags->zero);
	ft_putstr(cv->buffer_str);
	len = ft_strlen(cv->buffer_str);
	free(cv->buffer_str);
	free(cv->buffer_nb);
	free(cv);
	return (len);
}
