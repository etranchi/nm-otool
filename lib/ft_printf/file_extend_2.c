/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_extend_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/20 17:02:38 by etranchi          #+#    #+#             */
/*   Updated: 2017/10/20 17:02:40 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*check_sign(t_conv *cv)
{
	int		i;
	char	*tmp;

	i = 0;
	while ((cv->buffer_nb[i] >= '0' && cv->buffer_nb[i] <= '9') ||
		cv->buffer_nb[i] == '-' || cv->buffer_nb[i] == '+')
		i++;
	cv->buffer_nb[i] = '\0';
	if (cv->buffer_nb[0] == '-')
	{
		cv->sign = '-';
		tmp = cv->buffer_nb;
		cv->buffer_nb = ft_strdup(cv->buffer_nb + 1);
		free(tmp);
		return (cv->buffer_nb);
	}
	else
		cv->sign = '+';
	return (cv->buffer_nb);
}

void	f_minus(t_conv *cv, int pre)
{
	int i;
	int j;

	j = 0;
	i = 0;
	if (cv->sign == '-')
		cv->buffer_str[i++] = cv->sign;
	else
		cv->buffer_str[i++] = ' ';
	while (cv->mode && i < pre)
	{
		cv->buffer_str[i] = '0';
		i++;
	}
	while (cv->buffer_nb[j] && cv->buffer_str[i])
	{
		cv->buffer_str[i] = cv->buffer_nb[j];
		i++;
		j++;
	}
}

void	fill_moded_u(t_conv *cv, int f_plus, int space)
{
	int		i;
	int		j;
	char	*tmp;
	char	*tmp1;

	j = -1;
	i = 0;
	if (cv->sign == '-' || f_plus == 1)
	{
		cv->buffer_str[i++] = cv->sign;
	}
	else if (space == 1)
	{
		tmp = cv->buffer_str;
		tmp1 = ft_strdup(" ");
		cv->buffer_str = ft_strjoin(tmp1, cv->buffer_str);
		free(tmp);
		free(tmp1);
		i++;
	}
	while (cv->buffer_nb[++j])
		cv->buffer_str[i++] = cv->buffer_nb[j];
}

void	fill_moded_d(t_conv *cv, int f_plus, int pre)
{
	int i;
	int j;

	j = -1;
	i = 0;
	if (cv->sign == '-' && !f_plus && cv->empty != '0')
		cv->buffer_str[i - 1] = cv->sign;
	else if ((f_plus == 1 && cv->empty == '0') ||
		(cv->empty == '0' && cv->sign == '-'))
		cv->buffer_str[0] = cv->sign;
	else if (cv->sign == '-')
	{
		cv->buffer_str = ft_strjoin("-", cv->buffer_str);
		i++;
	}
	if (pre > 0)
		while ((pre - cv->buffer_len) > 0 && ++j < (pre - cv->buffer_len))
			cv->buffer_str[i++] = '0';
	j = -1;
	while (cv->buffer_nb[++j])
		cv->buffer_str[i++] = cv->buffer_nb[j];
	if (pre == (int)ft_strlen(cv->buffer_str) && cv->sign == '-')
		cv->buffer_str = ft_strjoin("-", cv->buffer_str);
}

int		moded_t_pre(t_conv *cv, int pre, int f_plus)
{
	int i;
	int j;

	j = -1;
	i = (int)ft_strlen(cv->buffer_str) - pre;
	if (cv->sign == '-' && f_plus == 0 && cv->empty != '0')
		cv->buffer_str[i - 1] = cv->sign;
	while ((pre - cv->buffer_len) > 0 && (pre - ft_strlen(cv->buffer_str)) > 0
		&& ++j < (pre - cv->buffer_len))
		cv->buffer_str[i++] = '0';
	i--;
	return (i);
}
