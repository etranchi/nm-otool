/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 17:32:27 by etranchi          #+#    #+#             */
/*   Updated: 2017/08/31 17:33:01 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# define UINT_MAX 4294967295;

# include "libft/libft.h"
# include <stdarg.h>
# include <unistd.h>
# include <sys/types.h>
# include <stdio.h>
# include <wchar.h>
# include <stdlib.h>
# include <limits.h>

typedef struct		s_flags
{
	int				diez;
	int				minus;
	int				plus;
	int				space;
	int				point;
	int				zero;
}					t_flags;

typedef struct		s_modifiers
{
	int				h;
	int				l;
	int				hh;
	int				ll;
	int				j;
	int				z;
}					t_modifiers;

typedef struct		s_conv
{
	char			*buffer_nb;
	wchar_t			*buffer_wnb;
	char			*buffer_str;
	int				buffer_len;
	int				mode;
	char			empty;
	char			sign;
}					t_conv;

typedef struct		s_env
{
	char			*fmt;
	char			*buffer;
	int				len;
	int				buff_len;
	int				pre;
	t_flags			*flags;
	t_modifiers		*modifiers;
}					t_env;

int					get_precision(t_env *e, int i, int *len);
int					get_buff_len(t_env *e, int i, int *len);
void				init_env(t_env *e, const char *format);
int					is_flag(char *str, int i);
int					is_modifier(char *str, int i);
int					is_type(char c);
int					set_modifiers(t_modifiers *modifiers, char *fmt, int i);
void				set_flags(t_flags *flags, char *fmt, int i);
int					set_type(t_env *e, char c, va_list params);
int					set_pourcent(t_env *e, char c);
void				init_flags_modi(t_flags *flags, t_modifiers *modifiers);
int					ft_conv_s(t_env *e, va_list params, char c);
int					ft_conv_c(t_env *e, va_list params, char c);
int					ft_conv_p(t_env *e, va_list params);
char				*ft_lutoa_base(unsigned long n, char *base);
char				*ft_itoa_base(unsigned int nb, char *base);
char				*ft_lltoa_base(long long nb, char *base);
char				*ft_llutoa_base(unsigned long long nb, char *base);
char				*ft_lltoa(long long nb);
char				*ft_llutoa(unsigned long long nb);
int					ft_conv_dec(t_env *e, va_list params, char c);
int					ft_conv_u(t_env *e, va_list params, char c);
int					ft_conv_x(t_env *e, va_list params, char c);
int					ft_conv_o(t_env *e, va_list params, char c);
char				*check_sign(t_conv *cv);
void				create_d_buffer(t_conv *cv, t_env *e);
void				create_s_buffer(t_conv *cv, t_env *e);
char				*ft_uitoa_base(unsigned int nb, char *base);
char				*ft_uitoa(unsigned int nb);
char				*ft_itoa(int nb);
void				f_fill_buff(t_conv *cv, int f_plus, int pre, int space);
void				fill_d_minus(t_conv *cv, int f_plus, int pre, int f_space);
void				fill_s_buffer(t_conv *cv, int pre);
void				fill_s_minus(t_conv *cv, int pre);
void				fill_x_buffer(t_conv *cv, int diez, int pre, char c);
void				fill_o_minus(t_conv *cv, t_flags *flags, int pre);
void				fill_o_buffer(t_conv *cv, int f_plus, int pre);
void				create_x_buffer(t_conv *cv, t_env *e);
int					ft_putwchar(wchar_t w, char *n);
char				*check_d_modifiers(t_modifiers *m, va_list params, char c);
void				check_s_modifiers(t_conv *cv, t_modifiers *m,
	va_list params, char c);
char				*check_u_modifiers(t_modifiers *m, va_list params, char c);
char				*check_x_modifiers(t_modifiers *m, va_list params, char c);
char				*check_o_modifiers(t_modifiers *m, va_list params, char c);
int					check_c_modifiers(t_modifiers *m, char c);
char				*check_p_modifiers(t_modifiers *m, va_list params);
void				free_cv(t_conv *cv);
void				create_c_buffer(t_conv *cv, t_env *e, int len);
void				fill_c_buffer(t_env *e, t_conv *cv, int nb);
char				*check_djz_modifiers(t_modifiers *m,
	va_list params, char c);
void				f_minus(t_conv *cv, int pre);
void				fill_moded_u(t_conv *cv, int f_plus, int space);
void				fill_moded_d(t_conv *cv, int f_plus, int pre);
void				fill_moded_t(t_conv *cv, int f_plus, int pre, int i);
void				check_width(t_env *e);
int					moded_t_pre(t_conv *cv, int pre, int f_plus);
int					ft_print_char(char c);
int					init_check(t_env *e, va_list params, int *i);
void				diez_o(t_conv *cv);
void				create_o_buffer(t_conv *cv, t_env *e);
void				fill_modeom_d(t_conv *cv, t_flags *flags, int pre);
void				fill_modeom_t(t_conv *cv, t_flags *flags, int pre);
void				fill_modeo_t(t_conv *cv, int f_plus, int pre);
void				fill_modeo_u(t_conv *cv);
void				create_bmode_u(t_conv *cv, t_env *e, int len);
void				diez_mode_bis(t_env *e, t_conv *cv, char *tmp);
void				diez(t_env *e, t_conv *cv, char c);
void				put_to_buff(char *str, char *tmp, int len);
void				fill_modex_d(t_conv *cv, int diez, int pre, char *tmp);
void				fill_modex_t(t_conv *cv, int diez, char *tmp);
void				fill_modes_u(t_conv *cv, int pre);
void				put_little_s(t_env *e, t_conv *cv, int *len, int nul);
int					ft_lllen(long long nb);
int					ft_ulllen(unsigned long long nb);
int					ft_uintlen(unsigned long long nb);
int					ft_intlen(unsigned long long nb);
char				*ft_lltoa(long long nb);
char				*ft_llutoa(unsigned long long nb);
char				*ft_uitoa(unsigned int nb);
char				*ft_itoa(int nb);
int					ui_base_len(unsigned int nb, unsigned int base);
char				*ft_uitoa_base(unsigned int nb, char *base);
int					ft_print_char(char c);
int					init_check(t_env *e, va_list params, int *i);

#endif
