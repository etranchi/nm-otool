/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Etienne <etranchi@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/16 17:08:39 by Etienne           #+#    #+#             */
/*   Updated: 2017/09/16 17:08:41 by Etienne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*check_o_modifiers(t_modifiers *m, va_list params, char c)
{
	char *buffer;
	char *base;

	base = ft_strdup("01234567");
	if (m->l || c == 'O')
		buffer = ft_llutoa_base(va_arg(params, unsigned long), base);
	else if (m->ll)
		buffer = ft_llutoa_base(va_arg(params, unsigned long long), base);
	else if (m->h)
		buffer = ft_llutoa_base((unsigned short)va_arg(params,
			unsigned int), base);
	else if (m->hh)
		buffer = ft_llutoa_base((unsigned char)va_arg(params,
			unsigned int), base);
	else if (m->j)
		buffer = ft_llutoa_base(va_arg(params, uintmax_t), base);
	else if (m->z)
		buffer = ft_llutoa_base(va_arg(params, size_t), base);
	else
		buffer = ft_uitoa_base(va_arg(params, unsigned int), base);
	free(base);
	return (buffer);
}

char	*check_u_modifiers(t_modifiers *m, va_list params, char c)
{
	char *buffer;

	if (m->l || c == 'U')
		buffer = ft_llutoa(va_arg(params, unsigned long));
	else if (m->ll)
		buffer = ft_llutoa(va_arg(params, unsigned long long));
	else if (m->h)
		buffer = ft_llutoa((unsigned short)va_arg(params, unsigned int));
	else if (m->hh)
		buffer = ft_llutoa((unsigned char)va_arg(params, unsigned int));
	else if (m->j)
		buffer = ft_llutoa(va_arg(params, uintmax_t));
	else if (m->z)
		buffer = ft_llutoa(va_arg(params, size_t));
	else
		buffer = ft_llutoa(va_arg(params, unsigned int));
	return (buffer);
}

char	*check_p_modifiers(t_modifiers *m, va_list params)
{
	char *buffer;

	if (m->l)
		buffer = ft_llutoa_base(va_arg(params, unsigned long),
			"0123456789abcdef");
	else
		buffer = ft_llutoa_base(va_arg(params, unsigned long long),
			"0123456789abcdef");
	return (buffer);
}

int		check_c_modifiers(t_modifiers *m, char c)
{
	if (c == 'c' && m->l == 0)
		return (1);
	else
		return (0);
}

void	check_s_modifiers(t_conv *cv, t_modifiers *m, va_list params, char c)
{
	cv->buffer_nb = NULL;
	cv->buffer_wnb = NULL;
	if (m->l || c == 'S')
	{
		cv->buffer_wnb = va_arg(params, wchar_t*);
	}
	else
	{
		cv->buffer_nb = va_arg(params, char*);
	}
}
