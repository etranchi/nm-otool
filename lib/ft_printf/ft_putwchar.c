/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putwchar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Etienne <etranchi@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/27 16:21:31 by Etienne           #+#    #+#             */
/*   Updated: 2017/09/27 16:21:33 by Etienne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	case_2(unsigned char *c, char *str)
{
	unsigned char c1;
	unsigned char c2;

	c1 = c[0];
	c2 = c[1];
	c2 <<= 2;
	c2 = c2 | (c1 >> 6);
	c1 = c1 & 0b00111111;
	c1 = c1 | 0b10000000;
	c2 = c2 & 0b00011111;
	c2 = c2 | 0b11000000;
	str[0] = c2;
	str[1] = c1;
}

static void	case_3(unsigned char *c, char *str)
{
	unsigned char			c1;
	unsigned char			c2;
	unsigned char			c3;

	c1 = c[0];
	c2 = c[1];
	c3 = c[2];
	c3 = c3 | (c2 >> 4);
	c3 = c3 & 0b00001111;
	c3 = c3 | 0b11100000;
	c2 = c2 << 2;
	c2 = c2 | (c1 >> 6);
	c1 = (c1 & 0b00111111) | 0b10000000;
	c2 = c2 & 0b00111111;
	c2 = c2 | 0b10000000;
	str[0] = c3;
	str[1] = c2;
	str[2] = c1;
}

static void	case_4(unsigned char *c, char *str)
{
	unsigned char			c1;
	unsigned char			c2;
	unsigned char			c3;
	unsigned char			c4;

	c1 = c[0];
	c2 = c[1];
	c3 = c[2];
	c4 = c[4];
	c4 = c4 | c3 >> 2;
	c3 = c3 << 4;
	c3 = c3 | (c2 >> 4);
	c3 = c3 & 0b00111111;
	c3 = c3 | 0b10000000;
	c2 = c2 << 2;
	c2 = c2 | (c1 >> 6);
	c1 = c1 & 0b00111111;
	c1 = c1 | 0b10000000;
	c2 = (c2 & 0b00111111) | 0b10000000;
	c4 = c4 & 0b00000111;
	c4 = c4 | 0b11110000;
	str[0] = c4;
	str[1] = c3;
	str[2] = c2;
	str[3] = c1;
}

void		write_unicode(char *str, int len)
{
	if (len >= 1)
		write(1, &str[0], 1);
	if (len >= 2)
		write(1, &str[1], 1);
	if (len >= 3)
		write(1, &str[2], 1);
	if (len >= 4)
		write(1, &str[3], 1);
}

int			ft_putwchar(wchar_t c, char *str)
{
	if (c >= 0 && c <= 128)
	{
		str[0] = c;
		write_unicode(str, 1);
		return (1);
	}
	else if (c > 0 && c <= 2048)
	{
		case_2((unsigned char*)&c, str);
		write_unicode(str, 2);
		return (2);
	}
	else if (c > 0 && c <= 65536)
	{
		case_3((unsigned char*)&c, str);
		write_unicode(str, 3);
		return (3);
	}
	else if (c > 0 && c <= INT_MAX)
	{
		case_4((unsigned char*)&c, str);
		write_unicode(str, 4);
		return (4);
	}
	return (-1);
}
