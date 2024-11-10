#include "../minishell.h"
// atoi_pointer LONG_MIN~LONG_MAXの数値か、NULL を返す

static bool	ft_isover(long long sign, long long num, long long next_num)
{
	if (num < LLONG_MAX / 10)
		return (false);
	if (num > LLONG_MAX / 10)
		return (true);
	if (sign == 1 && next_num > LLONG_MAX % 10)
		return (true);
	else if (sign == -1 && next_num > (LLONG_MIN % 10) * (-1))
		return (true);
	else
		return (false);
}

static long	*atol_pointer(const char *nptr)
{
	long long	num;
    long        *ptr;
	long long	sign;

	num = 0;
	sign = 1;
	if (*nptr == '-')
		sign = -1;
	if (*nptr == '+' || *nptr == '-')
		nptr++;
	while (ft_isdigit(*nptr))
	{
		if (sign == 1 && ft_isover(sign, num, (long)(*nptr - '0')))
			return (NULL);
		else if (sign == -1 && ft_isover(sign, num, (long)(*nptr - '0')))
			return (NULL);
		num = num * 10 + *nptr++ - '0';
	}
    num = sign * num;
    ptr = (long *)&num;
    if (*nptr && !ft_isdigit(*nptr))
        return (NULL);
    else
        return (ptr);
}

static int split_count(char **split)
{
    int i;

    i = 0;
    if (!split)
        return (0);
    else
    {
        while (split[i])
            i++;
    }
    return (i);
}

int    builtin_exit(char **split)
{
    long *result;

    result = NULL;
    if (split_count(split) == 1)
        return (0);
    else if (split_count(split) == 3)
        return (ft_printf(2, "exit\nbash: exit: too many arguments\n"), 1);
    else
    {
        result = atol_pointer(split[1]);
        if (result == NULL)
        {
            ft_printf(2, "bash: exit: %s", split[1]);
            ft_printf(2, ": numeric argument rewuired\n");
            return (2);
        }
        else
            return ((*result) % 256);
    }
}

bool main_exit(char *line, int *status)
{
    char    **split;

    split = NULL;
    if (ft_strchr(line, '|'))
        return (false);
    else
    {
        split = ft_split(line, ' ');
        if (!split)
            return (*status = 2, true);
        *status = builtin_exit(split);
            return (free_split(split), true);
    }
}