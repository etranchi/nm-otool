/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 17:31:25 by etranchi          #+#    #+#             */
/*   Updated: 2017/09/25 15:28:25 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		begin(t_env *e, va_list params)
{
	int i;

	i = 0;
	while (i < (int)ft_strlen(e->fmt) && e->fmt[i])
	{
		init_flags_modi(e->flags, e->modifiers);
		if (e->fmt[i] == '%')
		{
			if (init_check(e, params, &i) == 0)
				return (0);
		}
		else
			e->len += ft_print_char(e->fmt[i++]);
	}
	return (e->len);
}

void	release_env(t_env *e)
{
	free(e->fmt);
	free(e->flags);
	free(e->modifiers);
	free(e);
}

int		ft_printf(const char *format, ...)
{
	t_env		*e;
	va_list		params;
	int			len;

	if (!(e = (t_env *)malloc(sizeof(t_env))))
		return (-1);
	e->fmt = malloc(sizeof(char) * ((int)ft_strlen(format) + 1));
	e->fmt = ft_strcpy(e->fmt, format);
	e->flags = malloc(sizeof(t_flags));
	e->modifiers = malloc(sizeof(t_modifiers));
	e->buff_len = 0;
	e->pre = 0;
	e->len = 0;
	va_start(params, format);
	len = begin(e, params);
	va_end(params);
	release_env(e);
	return (len);
}

void	free_cv(t_conv *cv)
{
	if (cv->buffer_str)
		free(cv->buffer_str);
	if (cv->buffer_nb)
		free(cv->buffer_nb);
	free(cv);
}

void	init_env(t_env *e, const char *format)
{
	e->fmt = ft_strdup(format);
	e->flags = malloc(sizeof(t_flags));
	e->modifiers = malloc(sizeof(t_modifiers));
	e->buff_len = 0;
	e->pre = 0;
	e->len = 0;
}
