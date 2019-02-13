/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_extend_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/20 17:02:29 by etranchi          #+#    #+#             */
/*   Updated: 2017/10/20 17:02:33 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	create_c_buffer(t_conv *cv, t_env *e, int len)
{
	cv->mode = 0;
	if (e->buff_len > e->pre && e->buff_len > len)
	{
		cv->mode = 1;
		cv->buffer_str = malloc(sizeof(char) * (e->buff_len + 1));
		cv->buffer_str[e->buff_len] = '\0';
		if (e->pre > 0)
			cv->buffer_nb[e->pre] = '\0';
		ft_memset(cv->buffer_str, cv->empty, e->buff_len);
	}
	else if (e->pre > e->buff_len)
	{
		cv->mode = 2;
		cv->buffer_str = malloc(sizeof(char) * (e->pre + 1));
		cv->buffer_str[e->pre] = '\0';
	}
	else
	{
		cv->mode = 0;
		cv->buffer_str = malloc(sizeof(char) * (2));
		cv->buffer_str[1] = '\0';
		ft_memset(cv->buffer_str, cv->empty, 1);
	}
}

void	fill_c_buffer(t_env *e, t_conv *cv, int nb)
{
	int i;

	i = -1;
	if (e->flags->minus == 0)
	{
		while (++i < ((int)ft_strlen(cv->buffer_str) - 1))
			write(1, &cv->buffer_str[i], 1);
		ft_putchar(nb);
	}
	else
	{
		ft_putchar(nb);
		while (++i < ((int)ft_strlen(cv->buffer_str) - 1))
			write(1, &cv->buffer_str[i], 1);
	}
}

char	*check_djz_modifiers(t_modifiers *m, va_list params, char c)
{
	char *buffer;

	if (m->l == 1 || c == 'D')
		buffer = ft_lltoa(va_arg(params, long));
	else if (m->j && c != 'D')
		buffer = ft_lltoa(va_arg(params, intmax_t));
	else if (m->z && c != 'D')
		buffer = ft_lltoa(va_arg(params, size_t));
	else
		buffer = ft_itoa(va_arg(params, int));
	return (buffer);
}

char	*check_d_modifiers(t_modifiers *m, va_list params, char c)
{
	char	*buffer;
	int		tmp;

	if (m->ll && c != 'D')
		buffer = ft_lltoa(va_arg(params, long long));
	else if (m->hh && c != 'D')
	{
		if (!(tmp = (char)va_arg(params, int)))
			buffer = ft_strdup("0");
		else if (ft_strcmp((buffer = ft_itoa(tmp)), "128") == 0)
			buffer = ft_strdup("-128");
	}
	else if (m->h && c != 'D')
	{
		if (!(tmp = (short)va_arg(params, int)))
			buffer = ft_strdup("0");
		else if (ft_strcmp((buffer = ft_itoa(tmp)), "32768") == 0)
			buffer = ft_strdup("-32768");
	}
	else
		buffer = check_djz_modifiers(m, params, c);
	return (buffer);
}

char	*check_x_modifiers(t_modifiers *m, va_list params, char c)
{
	char *buffer;
	char *b;

	b = (c == 'x') ? ft_strdup("0123456789abcdef") :
	ft_strdup("0123456789ABCDEF");
	if (m->l)
		buffer = ft_llutoa_base(va_arg(params, unsigned long), b);
	else if (m->ll)
		buffer = ft_llutoa_base(va_arg(params, unsigned long long), b);
	else if (m->h)
		buffer = ft_llutoa_base((unsigned short)va_arg(params, unsigned int),
			b);
	else if (m->hh)
		buffer = ft_llutoa_base((unsigned char)va_arg(params, unsigned int), b);
	else if (m->j)
		buffer = ft_lutoa_base(va_arg(params, uintmax_t), b);
	else if (m->z)
		buffer = ft_lutoa_base(va_arg(params, size_t), b);
	else
		buffer = ft_llutoa_base(va_arg(params, unsigned int), b);
	free(b);
	return (buffer);
}
