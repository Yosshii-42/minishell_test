/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hurabe <hurabe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:46:06 by yotsurud          #+#    #+#             */
/*   Updated: 2024/12/05 08:41:17 by hurabe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "../minishell.h"

static char	*ft_strjoin_gnl(char *s1, char c, int len)
{
	char	*string;
	int		i;

	i = 0;
	if (!s1 && !c)
		return (NULL);
	string = NULL;
	string = (char *)safe_malloc(len, sizeof(char));
	i = -1;
	while (++i < len - 2)
		string[i] = s1[i];
	string[i] = c;
	string[++i] = '\0';
	free(s1);
	return (string);
}

//void	buf_zero(char *buf, char *ptr, int *read_byte)
static	void	buf_zero(char *buf, char **ptr, int *read_byte)
{
	int	i;

	i = -1;
	while (++i < BUFFER_SIZE)
		buf[i] = 0;
	*ptr = NULL;
	//*ptr = 0;
	//ptr = NULL;
	*read_byte = 0;
}

static char	ft_getchar(int fd)
{
	static char	buf[BUFFER_SIZE];
	//static char	*ptr;
	static char *ptr = NULL;
	static int	read_byte;

	if (read_byte == 0)
	{
		read_byte = read(fd, buf, BUFFER_SIZE);
		if (read_byte < 0)
			return (buf_zero(buf, &ptr, &read_byte), FAILE); 
			//return (buf_zero(buf, ptr, &read_byte), FAILE);
		ptr = buf;
	}
	if (--read_byte >= 0)
		return ((char)*(ptr++));
	return (buf_zero(buf, &ptr, &read_byte), EOF);
	//return (buf_zero(buf, ptr, &read_byte), EOF);
}

char	*get_next_line(int fd)
{
	char	*result;
	char	c;
	int		len;

	if (fd == -1)
		return (NULL);
	result = NULL;
	c = '\0';
	len = 2;
	while (1)
	{
		c = ft_getchar(fd);
		if (c == FAILE)
			return (free(result), NULL);
		if (c == EOF)
			break ;
		result = ft_strjoin_gnl(result, c, len);
		len++;
		if (c == '\n')
			break ;
	}
	return (result);
}
//
// __attribute__((destructor)) static void destructor()
// {
//     system("leaks -q a.out");
// }
//
//  int	main()
// {
//  	int		fd;
//  	char	*result;
//
//  	fd = 0;//open("test.txt", O_RDONLY);
// 	while(1)
//  	{
//  		result = get_next_line(fd);
//  		if (!result)
//  			break ;
//  		printf("%s", result);
//  		free(result);
//  	}
//  	close(fd);
// }
