/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envariements.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sael-kha <sael-kha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:02:14 by sael-kha          #+#    #+#             */
/*   Updated: 2025/06/02 16:25:42 by sael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*init_env(char **envp)
{
	t_env	*env;
	int 	i;
	int		count;

	count = 0;
	while (envp[count])
		count++;
	env = malloc(sizeof(t_env));
	env->vars = malloc(sizeof(char *) * (count + 1));
	i = 0;
	while (i < count)
	{
		env->vars[i] = ft_strdup(envp[i]);
		i++;
	}
	env->vars[count] = NULL;
	env->count = count;
	env->exit_s = 0;
	return (env);
}

char	*ft_getenv(t_env *env, const char *key)
{
	int		key_len;
	int		i;

	key_len = ft_strlen(key);
	i = 0;
	while (i < env->count)
	{
		if (ft_strncmp(env->vars[i], key, key_len) == 0
			&& env->vars[i][key_len] == '=')
			return (env->vars[i] + key_len + 1);
		i++;
	}
	return (NULL);
}
