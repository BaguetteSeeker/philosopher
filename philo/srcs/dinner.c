/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 10:01:54 by epinaud           #+#    #+#             */
/*   Updated: 2025/06/27 13:01:08 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

size_t	is_dinner_done(void)
{
	t_dinner	*dinner;

	dinner = gset_dinner(0);
	pthread_mutex_lock(&dinner->coordinator);
	if (dinner->is_done)
	{
		pthread_mutex_unlock(&dinner->coordinator);
		return (true);
	}
	pthread_mutex_unlock(&dinner->coordinator);
	return (false);
}

void	launch_dinner(t_guest *thinker, t_dinner dinner)
{
	size_t	i;

	if (pthread_mutex_init(&dinner.coordinator, NULL) != 0
		|| pthread_mutex_init(&dinner.display_lock, NULL) != 0)
		put_err("Failled to initialize mutex");
	if (dinner.guest_count == 1)
	{
		pthread_create(&thinker->thread, NULL, dine_alone, thinker);
		return ;
	}
	i = 0;
	while (i++ < dinner.guest_count)
	{
		thinker->last_meal = gset_dinner(0)->start_time;
		pthread_create(&thinker->thread, NULL, launch_routine, thinker);
		thinker = thinker->next;
	}
}

int	set_table(int argc, char *args[])
{
	size_t				val;
	static size_t		i = 0;
	static size_t		j = 0;
	t_dinner			*dinner;

	dinner = gset_dinner(0);
	while (args[i])
	{
		j = 0;
		while (args[i][j])
			if (!ft_isdigit(args[i][j++]))
				put_err("Given parameter is not a number");
		val = ft_atoi(args[i]);
		if (i == 0)
			dinner->guest_count = val;
		else if (i == 1)
			dinner->life_duration = val;
		else if (i == 2)
			dinner->meal_duration = val;
		else if (i == 3)
			dinner->sleep_duration = val;
		else if (i == 4)
			dinner->meals_required = val;
		i++;
		dinner->argc = argc;
	}
	return (1);
}

void	cleanup_table(t_dinner	*dinner, t_guest *philos)
{
	t_guest			*next_philo;
	size_t			i;

	i = 0;
	while (i++ < dinner->guest_count)
	{
		next_philo = philos->next;
		pthread_mutex_destroy(&philos->fork_mutex);
		free(philos);
		philos = next_philo;
	}
}
