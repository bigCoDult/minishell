/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanbaek <sanbaek@student.42gyeongsan.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 21:09:53 by sanbaek           #+#    #+#             */
/*   Updated: 2024/03/25 21:09:55 by sanbaek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# define DIGIT_RANGE_START			48
# define DIGIT_RANGE_END			57
# define UPPER_ALPHA_RANGE_START	65
# define UPPER_ALPHA_RANGE_END		90
# define LOWER_ALPHA_RANGE_START	97
# define LOWER_ALPHA_RANGE_END		122
# define ASCII_RANGE_START			0
# define ASCII_RANGE_END			127
# define PRINTABLE_RANGE_START		32
# define PRINTABLE_RANGE_END		126
# define LETTER_CASE_INTERVAL		32

typedef struct s_etc
{
	int						n;
	unsigned int			u;
	char					*string;
	long long unsigned int	*ads;
}	t_etc;

typedef struct s_min_info
{
	int		minus_flag;
	int		length;
	int		n1;
	int		n2;
}	t_min_info;

typedef struct s_sentence
{
	size_t	w_c;
	size_t	*edge_pair;
	char	**words;
}	t_sentence;

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

int		ft_isdigit(int c);
int		ft_isalpha(int c);
int		ft_isalnum(int c);
int		ft_isprint(int c);
int		ft_isascii(int c);
int		ft_tolower(int c);
int		ft_toupper(int c);
size_t	ft_strlen(const char *s);
void	*ft_memset(void *dest, int c, size_t count);
void	ft_bzero(void *dest, size_t count);
void	*ft_memcpy(void *dest, const void *src, size_t count);
int		ft_memcmp(const void *lhs, const void *rhs, size_t size);
char	*ft_strnstr(const char *big, const char *little, size_t len);
void	*ft_memmove(void *dest, const void *src, size_t count);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
size_t	ft_strlcat(char *dest, const char *src, size_t size);
char	*ft_strchr(const char *str, int c);
char	*ft_strrchr( const char *str, int c );
int		ft_strncmp( const char *lhs, const char *rhs, size_t count );
void	*ft_memchr( const void *ptr, int c, size_t count );
void	*ft_calloc( size_t num, size_t size );
char	*ft_strdup(const char *src);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_itoa(int n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
int		ft_printf(const char *str, ...);
int		ft_printffmt_cdux(t_etc *etc, va_list ap, char col);
int		ft_printffmt_sp(t_etc *etc, va_list ap, char col);
int		ft_printfchar_fd(char c, int fd);
int		ft_printfstr_fd(char *s, int fd);
int		ft_printfll_fd(long long int n, int fd);
int		ft_printfhex_fd(int islower, long long unsigned int n, int fd);
char	**ft_split(char const *str, char separator);
int		ft_isspace(char c);
int		ft_atoi(char *str);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new);
void	ft_lstadd_back(t_list **lst, t_list *new);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstclear(t_list **lst, void (*del)(void*));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

#endif /* LIBFT_H */