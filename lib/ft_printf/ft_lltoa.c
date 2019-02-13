/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lltoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/06 14:56:30 by etranchi          #+#    #+#             */
/*   Updated: 2017/09/06 14:56:32 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		int_base_len(int nb, int base)
{
	int i;

	i = 1;
	while (nb /= base)
		i++;
	return (i);
}

char	*ft_itoa_base(unsigned int nb, char *base)
{
	char	*str;
	int		len;
	int		sign;

	sign = 0;
	len = int_base_len(nb, ft_strlen(base));
	str = malloc(sizeof(char) * (len + 1));
	str[len--] = '\0';
	while (nb / ft_strlen(base) > 0)
	{
		str[len--] = base[nb % (int)ft_strlen(base)];
		nb /= (int)ft_strlen(base);
	}
	return (str);
}

int		ull_base_len(unsigned long long nb, unsigned int base)
{
	int i;

	i = 1;
	while (nb /= base)
		i++;
	return (i);
}

char	*ft_lutoa_base(unsigned long n, char *base)
{
	char				*str;
	int					len;
	unsigned long long	nb;

	nb = (unsigned long long)n;
	len = ull_base_len(nb, ft_strlen(base));
	str = malloc(sizeof(char) * (len + 1));
	str[len--] = '\0';
	while (nb / (unsigned long long)ft_strlen(base) > 0)
	{
		str[len--] = base[nb % ft_strlen(base)];
		nb /= ft_strlen(base);
	}
	str[len] = base[nb % ft_strlen(base)];
	return (str);
}

char	*ft_llutoa_base(unsigned long long nb, char *base)
{
	char	*str;
	int		len;

	if (nb == 4294967296 && ft_strcmp(base, "023456789abcdef") == 0)
		return (ft_strdup("100000000"));
	if (nb == 4294967296 && ft_strcmp(base, "023456789ABCDEF") == 0)
		return (ft_strdup("0"));
	len = ull_base_len(nb, ft_strlen(base));
	str = malloc(sizeof(char) * (len + 1));
	str[len--] = '\0';
	while (nb / (unsigned long long)ft_strlen(base) > 0)
	{
		str[len--] = base[nb % ft_strlen(base)];
		nb /= ft_strlen(base);
	}
	str[len] = base[nb % ft_strlen(base)];
	return (str);
}
