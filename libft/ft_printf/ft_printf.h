/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 16:54:17 by hubourge          #+#    #+#             */
/*   Updated: 2025/02/10 17:07:15 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>

int	ft_printf(int fd, const char *str, ...);
int	ft_pf_strlen(const char *str);
int	ft_pf_putchar_fd(char c, int fd);
int	ft_pf_putstr_fd(char *s, int fd);
int	ft_pf_putnbr_fd(long long int n, int fd, int cpt);
int	ft_pf_putnbr_uns_fd(unsigned int n, int fd, int cpt);
int	ft_pf_putnbr_adr(unsigned long n, char *base, unsigned int len, int cpt, int fd);
int	ft_pf_putnbr_base(int nbr, char *base, unsigned int len, int cpt, int fd);
int	ft_pf_address(unsigned long ptr, int cpt, int fd);
int	ft_pf_check(long n, int cpt, int fd);
int	ft_pf_putnbr_st_fd(size_t n, int fd, int cpt);

#endif