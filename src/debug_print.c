#include "minishell.h"
#include <stdarg.h>

typedef struct s_etc
{
	int						n;
	unsigned int			u;
	char					*string;
	long long unsigned int	*ads;
}	t_etc;
static int	ft_vprintf(const char *str, va_list *ap);
static int	ft_printffmt_cdux(t_etc *etc, va_list *ap, char c);
static int	ft_printffmt_sp(t_etc *etc, va_list *ap, char c);
static int	ft_printfchar_fd(char col, int fd);
static int	ft_printfstr_fd(char *s, int fd);
static int	ft_printfll_fd(long long int n, int fd);
static int	ft_printfhex_fd(int is_x, long long unsigned int n, int fd);

// 	모두출력 = 2047

// 이 함수의 목적은
// 	<debug_printf(n> 를 ctrl+shift+f로 검색하여 "일괄 변경"하는 것으로
// 	전체 프로세스에서 어떤 디버깅 문자열의 출력될지를 바꾸는 바꾸는 것입니다

// 사용법
// 	action_combine_bit는 2의 0승, 1승, 2승, 3승, 4승의 합이며 각 비트는 출력할 문자열을 결정합니다.
// 	action_combine_bit가 3이면 2의 0승 + 1승이므로 str타입과 env타입인 문자열을 출력합니다.

// 아래의 경우, 타입 token과 ast에 해당하는 문자열만 출력합니다.
// debug_printf(12, 3, "token is %s, token->value);
// debug_printf(12, 4, "ast is %s, ast->value);


// str_type 1 = 단순 문자열
// action_combine_bit 1

// str_type 2 = 환경변수 관련
// action_combine_bit 2

// str_type 3 = 토큰 관련
// action_combine_bit 4

// str_type 4 = AST 관련
// action_combine_bit 8

// str_type 5 = 위의 타입에 해당하지 않으며, 다른 구체적인 타입을 필요로함, 추후 재정의 할것
// action_combine_bit 16

// str_type 6 = path 확인
// action_combine_bit 32

// str_type 7 = 진행상황
// action_combine_bit 64

// str_type 8 = 파이프
// action_combine_bit 128

// str_type 9 = 히어독
// action_combine_bit 256

// str_type 10 = 리다이렉션
// action_combine_bit 512

// str_type 11 = memory
// action_combine_bit 1024


void debug_print(int action_combine_bit, int str_type, const char *str, ...)
{
	va_list *ap;
	int	mask;
	int	type;
	// signal(SIGPIPE, SIG_IGN);

	// if (action_combine_bit == 0)
	// 	return ;

	type = 1;
	mask = 1;
	ap = malloc(sizeof(va_list));
	if (ap == NULL)
		return ;
	va_start(*ap, str);
	while (mask < 2048)
	{
		
		// printf("%d ", mask);
		
		if ((action_combine_bit & mask) && str_type == type)
		{
			// printf("type = %d\n", type);
			ft_vprintf(str, ap);
		}
		mask <<= 1;
		type++;
		
		// if (mask == 2048)
		// 	printf("%d\n", mask);
	
	}
	va_end(*ap);
	free(ap);
}
















static int	ft_vprintf(const char *str, va_list *ap)
{
	t_etc	*etc;
	int		final_count;

	etc = malloc(sizeof(t_etc));
	if (etc == NULL)
		return (-1);
	etc->n = 0;
	etc->u = 0;
	etc->string = "";
	etc->ads = NULL;
	while (*str)
	{
		if (*str == '%')
			ft_printffmt_cdux(etc, ap, *++str);
		else
			etc->n += ft_printfchar_fd(*str, 1);
		str++;
	}
	final_count = etc->n;
	free(etc);
	return (final_count);
}


static int	ft_printffmt_cdux(t_etc *etc, va_list *ap, char c)
{
	if (c == '%')
		etc->n += ft_printfchar_fd('%', 1);
	else if (c == 'c')
		etc->n += ft_printfchar_fd(va_arg(*ap, int), 1);
	else if (c == 'd' || c == 'i')
		etc->n += ft_printfll_fd(va_arg(*ap, int), 1);
	else if (c == 'u')
		etc->n += ft_printfll_fd(va_arg(*ap, unsigned int), 1);
	else if (c == 'x' || c == 'X')
		etc->n += ft_printfhex_fd(c, va_arg(*ap, unsigned int), 1);
	else
		return (ft_printffmt_sp(etc, ap, c));
	return (etc->n);
}

static int	ft_printffmt_sp(t_etc *etc, va_list *ap, char c)
{
	if (c == 's')
	{
		etc->string = (va_arg(*ap, char *));
		if (etc->string == NULL)
			etc->n += ft_printfstr_fd("(null)", 1);
		else
			etc->n += ft_printfstr_fd(etc->string, 1);
	}
	else if (c == 'p')
	{
		etc->ads = va_arg(*ap, void *);
		if (etc->ads == NULL)
			etc->n += ft_printfstr_fd("(nil)", 1);
		else
		{
			etc->n += ft_printfstr_fd("0x", 1);
			etc->n += ft_printfhex_fd('x', (long long unsigned int)etc->ads, 1);
		}
	}
	return (etc->n);
}

static int	ft_printfchar_fd(char col, int fd)
{
	write(fd, &col, 1);
	return (1);
}

static int	ft_printfstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
	return (ft_strlen(s));
}

static int	ft_printfll_fd(long long int n, int fd)
{
	int	count;

	count = 1;
	if (n < 0)
	{
		n *= -1;
		write(fd, "-", 1);
		count++;
	}
	if (n > 9)
		count += ft_printfll_fd(n / 10, fd);
	n = n % 10 + '0';
	write(fd, &n, 1);
	return (count);
}

static int	ft_printfhex_fd(int is_x, long long unsigned int n, int fd)
{
	char	digit;
	int		count;

	digit = '\0';
	count = 1;
	if (n >= 16)
		count += ft_printfhex_fd(is_x, n / 16, fd);
	n = n % 16;
	if (n < 10)
		digit = n + '0';
	else if (is_x == 'x')
		digit = n + 'a' - 10;
	else if (is_x == 'X')
		digit = n + 'A' - 10;
	write(fd, &digit, 1);
	return (count);
}