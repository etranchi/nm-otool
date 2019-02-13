/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conv_s.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/06 15:02:41 by etranchi          #+#    #+#             */
/*   Updated: 2017/09/06 15:02:43 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		get_len(wchar_t *str)
{
	int i;
	int len;

	i = 0;
	len = 0;
	while (str[i])
	{
		if (str[i] > 0 && str[i] <= 128)
			len += 1;
		else if (str[i] > 0 && str[i] <= 2048)
			len += 2;
		else if (str[i] > 0 && str[i] <= 65536)
			len += 3;
		else if (str[i] > 0 && str[i] <= INT_MAX)
			len += 4;
		else
			break ;
		i++;
	}
	return (len);
}

void	put_big_val2_s(t_env *e, t_conv *cv, int *len)
{
	int i;
	int j;

	j = 0;
	i = 0;
	while (i < (e->buff_len - (*len)))
	{
		write(1, &cv->empty, 1);
		i++;
	}
	e->buff_len = i;
	i = 0;
	while ((*len) > 0 && i < (*len) && cv->buffer_wnb[i] != 0)
	{
		j += ft_putwchar(cv->buffer_wnb[i], cv->buffer_str);
		i++;
	}
	(*len) = j;
}

void	put_big_val_s(t_env *e, t_conv *cv, int *len)
{
	int i;
	int j;

	i = 0;
	*len = get_len(cv->buffer_wnb);
	if (e->pre > e->buff_len)
		*len = e->pre - 1;
	if (e->buff_len > e->pre && e->pre)
		(*len) = e->pre;
	if (e->flags->minus == 1)
	{
		j = 0;
		while ((*len) > 0 && j < (*len) && cv->buffer_wnb[j] != 0)
		{
			i += ft_putwchar(cv->buffer_wnb[j], cv->buffer_str);
			j++;
		}
		while ((i++) < e->buff_len)
			write(1, &cv->empty, 1);
		(*len) = i - 1;
	}
	else
		put_big_val2_s(e, cv, len);
}

void	put_big_s(t_env *e, t_conv *cv, int *len)
{
	int i;

	i = 0;
	cv->buffer_str = ft_strdup("    ");
	if (e->flags->zero && e->flags->point)
	{
		create_s_buffer(cv, e);
		ft_putstr(cv->buffer_str);
		(*len) = ft_strlen(cv->buffer_str);
	}
	else if (cv->buffer_wnb == 0 || cv->buffer_wnb == NULL)
	{
		cv->buffer_nb = ft_strdup("(null)");
		ft_putstr(cv->buffer_nb);
		*len = ft_strlen(cv->buffer_nb);
	}
	else if (cv->buffer_wnb != NULL)
		put_big_val_s(e, cv, len);
	free(cv->buffer_str);
}

int		ft_conv_s(t_env *e, va_list params, char c)
{
	t_conv	*cv;
	int		len;
	int		nul;

	nul = 0;
	cv = malloc(sizeof(t_conv));
	cv->empty = (e->flags->zero) ? '0' : ' ';
	len = 0;
	check_s_modifiers(cv, e->modifiers, params, c);
	if (cv->buffer_nb == NULL && (e->pre > 0 || e->buff_len > 0))
		cv->buffer_nb = ft_strdup("");
	else if (cv->buffer_nb == NULL && e->pre == 0 && (c == 's'
		&& e->modifiers->l == 0))
	{
		cv->buffer_nb = ft_strdup("(null)");
		nul++;
	}
	if (e->flags->point == 1 && e->pre == 0)
		cv->buffer_nb = ft_strdup("");
	if ((e->modifiers->l == 0 && c != 'S'))
		put_little_s(e, cv, &len, nul);
	else
		put_big_s(e, cv, &len);
	free(cv);
	return (len);
}
