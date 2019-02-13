/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_extend_8.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/20 17:16:26 by etranchi          #+#    #+#             */
/*   Updated: 2017/10/20 17:16:30 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_lllen(long long nb)
{
	int i;

	i = 1;
	while (nb /= 10)
		i++;
	return (i);
}

int		ft_ulllen(unsigned long long nb)
{
	int i;

	i = 1;
	while (nb /= 10)
		i++;
	return (i);
}

int		ft_uintlen(unsigned long long nb)
{
	int i;

	i = 1;
	while (nb /= 10)
		i++;
	return (i);
}

int		ft_intlen(unsigned long long nb)
{
	int i;

	i = 1;
	while (nb /= 10)
		i++;
	return (i);
}

char	*ft_lltoa(long long nb)
{
	char	*str;
	int		sign;
	int		len;

	sign = 0;
	if ((unsigned long long)nb == -9223372036854775808ull)
		return (ft_strdup("-9223372036854775808"));
	if ((long long)nb < 0 && nb != 2147483648)
	{
		sign = 1;
		nb = -nb;
	}
	len = ft_lllen(nb) + sign;
	str = malloc(sizeof(char) * (len + 1));
	str[len] = '\0';
	while (len--)
	{
		str[len] = nb % 10 + '0';
		nb = nb / 10;
	}
	if (sign == 1)
		str[0] = '-';
	return (str);
}
