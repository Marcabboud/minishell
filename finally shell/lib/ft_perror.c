/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_perror.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabboud <mabboud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:23:30 by mabboud           #+#    #+#             */
/*   Updated: 2025/03/28 13:24:51 by mabboud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_putchar(char c)
{
	write(2, &c, 1);
}

void	ft_perror(const char *str, char *tok)
{
	while (*str)
		ft_putchar(*str++);
	if (tok)
	{
		ft_putchar('`');
		while (*tok)
			ft_putchar(*tok++);
		ft_putchar('\'');
		ft_putchar('\n');
	}
}
