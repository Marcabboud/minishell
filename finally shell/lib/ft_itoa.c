/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabboud <mabboud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:23:15 by mabboud           #+#    #+#             */
/*   Updated: 2025/03/28 13:24:51 by mabboud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <stdlib.h>

static size_t	size_nb(long long int nb)
{
	size_t	size;

	size = 0;
	if (nb == 0)
		return (1);
	else if (nb < 0)
	{
		nb = nb * -1;
		size++;
	}
	while (nb > 0)
	{
		nb = nb / 10;
		size++;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	char			*new;
	long long int	nb;
	size_t			size;

	nb = n;
	size = size_nb(nb);
	new = malloc(sizeof(char) * size + 1);
	if (!new)
		return (0);
	new[size--] = '\0';
	if (nb == 0)
		new[0] = 48;
	else if (nb < 0)
	{
		new[0] = '-';
		nb = nb * -1;
	}
	while (nb > 0)
	{
		new[size--] = (nb % 10) + 48;
		nb = nb / 10;
	}
	return (new);
}
