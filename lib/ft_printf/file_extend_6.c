/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_extend_6.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/20 17:16:09 by etranchi          #+#    #+#             */
/*   Updated: 2017/10/20 17:16:13 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	diez_mode_bis(t_env *e, t_conv *cv, char *tmp)
{
	int i;

	i = 0;
	if (e->flags->zero)
	{
		cv->buffer_str[0] = tmp[0];
		cv->buffer_str[1] = tmp[1];
	}
	else
	{
		i = ft_strlen(cv->buffer_str) - ft_strlen(cv->buffer_nb);
		cv->buffer_str[i - 2] = tmp[0];
		cv->buffer_str[i - 1] = tmp[1];
	}
}

void	diez(t_env *e, t_conv *cv, char c)
{
	char *tmp;

	tmp = (c == 'x') ? ft_strdup("0x") : ft_strdup("0X");
	if (ft_strlen(cv->buffer_str) > ft_strlen(cv->buffer_nb) && e->flags->minus)
	{
		cv->buffer_str[0] = tmp[0];
		cv->buffer_str[1] = tmp[1];
	}
	else if (ft_strlen(cv->buffer_str) > ft_strlen(cv->buffer_nb) &&
		!e->flags->minus)
		diez_mode_bis(e, cv, tmp);
	else if (ft_strlen(cv->buffer_nb) == ft_strlen(cv->buffer_str))
		cv->buffer_str = ft_strjoin(tmp, cv->buffer_nb);
}

void	put_to_buff(char *str, char *tmp, int len)
{
	str[len] = tmp[1];
	str[len - 1] = tmp[0];
}

void	fill_modex_d(t_conv *cv, int diez, int pre, char *tmp)
{
	int i;
	int j;

	i = -1;
	j = (int)ft_strlen(cv->buffer_str) - (int)ft_strlen(cv->buffer_nb) - 1;
	if (diez == 1 && ft_strcmp("0", cv->buffer_nb) != 0 && cv->empty == ' ')
		put_to_buff(cv->buffer_str, tmp, j);
	else if (diez == 1 && ft_strcmp("0", cv->buffer_nb) != 0
		&& cv->empty == '0')
		put_to_buff(cv->buffer_str, tmp, 1);
	if (pre > (int)ft_strlen(cv->buffer_nb))
	{
		j -= (pre - (int)ft_strlen(cv->buffer_nb));
		while ((pre - cv->buffer_len) > 0 && ++i < pre - cv->buffer_len)
			cv->buffer_str[++j] = '0';
		j -= 2;
	}
	i = -1;
	while (cv->buffer_nb[++i])
		cv->buffer_str[++j] = cv->buffer_nb[i];
}

void	fill_modex_t(t_conv *cv, int diez, char *tmp)
{
	char	*tmp1;
	int		i;
	int		j;

	i = -1;
	j = ft_strlen(cv->buffer_str) - ft_strlen(cv->buffer_nb);
	while (cv->buffer_nb[++i])
		cv->buffer_str[j++] = cv->buffer_nb[i];
	if (diez == 1 && ft_strcmp("0", cv->buffer_nb) != 0)
	{
		tmp1 = cv->buffer_str;
		cv->buffer_str = ft_strjoin(tmp, cv->buffer_str);
		free(tmp1);
	}
}
