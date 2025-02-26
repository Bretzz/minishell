/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_stolen.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <topiana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:37:02 by topiana-          #+#    #+#             */
/*   Updated: 2025/02/26 20:42:15 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_env_path(char *cmd, char **env);

/* strjoin without stlcpy and strlcat
and frees s1 */
static char	*ft_strjoin_free(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*twelve;
	size_t	size;

	size = ft_strlen(s1) + ft_strlen(s2) + 1;
	twelve = (char *)malloc(size * sizeof(char));
	if (twelve == NULL)
		return (NULL);
	i = -1;
	while (s1[++i])
		twelve[i] = s1[i];
	j = 0;
	while (s2[j])
		twelve[i++] = s2[j++];
	twelve[i] = '\0';
	free(s1);
	return (twelve);
}

/*takes a string 'str', a separetor 'c' and a tail-string 'tail' as parameters 
Then splits the string based on the separetor,
mallocating both the array and the single strings
Then append the tail-string at the end of each array-string
RETURNS: the mallocated array, NULL on error*/
static char	**ft_split_tail(char *str, char c, char *tail)
{
	int		i;
	char	**arr;

	arr = ft_split(str, c);
	if (arr == NULL)
		return (NULL);
	i = 0;
	while (arr[i] != NULL)
	{
		arr[i] = ft_strjoin_free(arr[i], tail);
		if (arr[i] == NULL)
		{
			ft_free_arr(arr);
			return (NULL);
		}
		i++;
	}
	return (arr);
}

/* takes a 'command' string and the env variables as parameters
searches the 'PATH' enviroment variable
then searches if any of the env-path contains the command's file
RETURNS: a mallocated string with the command's path, NULL on error */
char	*find_env_path(char *cmd, char **env)
{
	int		i;
	char	**paths;
	char	*cmd_path;

	i = 0;
	while (env[i] && ft_strncmp("PATH", env[i], 4))
		i++;
	cmd = ft_strjoin("/", cmd);
	if (env[i] == NULL)
		return (NULL);
	paths = ft_split_tail(env[i] + 5, ':', cmd);
	if (!paths || !cmd)
		return (NULL);
	cmd_path = NULL;
	i = 0;
	while (paths[i] != NULL && access(paths[i], X_OK) != 0)
		i++;
	cmd_path = ft_strdup(paths[i]);
	ft_free_arr(paths);
	free(cmd);
	return (cmd_path);
}
