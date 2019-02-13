/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_extend_9.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/20 17:16:34 by etranchi          #+#    #+#             */
/*   Updated: 2017/10/20 17:16:35 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_llutoa(unsigned long long nb)
{
	char	*str;
	int		len;

	len = ft_ulllen(nb);
	str = malloc(sizeof(char) * (len + 1));
	str[len] = '\0';
	while (len--)
	{
		str[len] = nb % 10 + '0';
		nb = nb / 10;
	}
	return (str);
}

char	*ft_uitoa(unsigned int nb)
{
	char	*str;
	int		len;

	len = ft_uintlen(nb);
	str = malloc(sizeof(char) * (len + 1));
	str[len] = '\0';
	while (len--)
	{
		str[len] = nb % 10 + '0';
		nb = nb / 10;
	}
	return (str);
}

char	*ft_itoa(int nb)
{
	char	*str;
	int		sign;
	int		len;

	sign = 0;
	if (nb == -2147483648)
		return (ft_strdup("-2147483648"));
	if (nb < 0)
	{
		sign = 1;
		nb = -nb;
	}
	len = ft_intlen(nb) + sign;
	str = malloc(sizeof(char) * (len + 1));
	str[len] = '\0';
	while (len--)
	{
		str[len] = nb % 10 + '0';
		nb = nb / 10;
	}
	if (sign)
		str[0] = '-';
	return (str);
}

int		ui_base_len(unsigned int nb, unsigned int base)
{
	int i;

	i = 1;
	while (nb /= base)
		i++;
	return (i);
}

char	*ft_uitoa_base(unsigned int nb, char *base)
{
	char	*str;
	int		len;

	len = ui_base_len(nb, ft_strlen(base));
	str = malloc(sizeof(char) * (len + 1));
	str[len--] = '\0';
	while ((nb / ((unsigned int)ft_strlen(base))) > 0)
	{
		str[len--] = base[nb % ft_strlen(base)];
		nb /= ft_strlen(base);
	}
	str[len] = base[nb % ft_strlen(base)];
	return (str);
}
