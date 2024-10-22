/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfinet <gfinet@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:30:39 by gfinet            #+#    #+#             */
/*   Updated: 2024/03/04 22:24:58 by gfinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>
# include "libft/libft.h"

int		ft_printf(const char *arg0, ...);
char	*ft_strndup(const char *src, size_t n);
char	*ft_ltoa_base(long int n, char *base);
char	*ft_ultoa_base(unsigned long n, char *base);
int		check_conv(char c);
char	*string_test(char *s);
char	*pointer_modif(unsigned long arg);

typedef struct s_print
{
	char			*content;
	char			type;
	struct s_print	*next;
}	t_print;

int		ft_make_list(const char *arg0, t_print **p_list,
			int nb_arg, va_list ap);

void	ft_plstadd_back(t_print **lst, t_print *ne);
int		ft_plstclear(t_print **lst, void (*del)(void *));
void	ft_plstdelone(t_print *lst, void (*del)(void *));
t_print	*ft_plstnew(char *cont);

#endif