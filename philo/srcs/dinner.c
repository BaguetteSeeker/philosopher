/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 10:01:54 by epinaud           #+#    #+#             */
/*   Updated: 2025/07/01 18:27:35 by epinaud          ###   ########.fr       */
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

// printf("Launched philo %ld\n", thinker->id);
static void	launch_philo_batch(t_guest *thinker, size_t batch_size)
{
	size_t	i;

	i = 0;
	if (thinker->id != 1)
		i++;
	while (i < batch_size)
	{
		thinker->last_meal = gset_dinner(0)->start_time;
		pthread_create(&thinker->thread, NULL, launch_routine, thinker);
		thinker = thinker->next->next;
		i += 2;
	}
}

void	launch_dinner(t_guest *thinker, t_dinner dinner)
{
	t_guest	*even_thinkers;

	even_thinkers = thinker->next;
	if (pthread_mutex_init(&dinner.coordinator, NULL) != 0
		|| pthread_mutex_init(&dinner.display_lock, NULL) != 0)
		put_err("Failled to initialize mutex");
	if (dinner.guest_count == 1)
	{
		pthread_create(&thinker->thread, NULL, dine_alone, thinker);
		return ;
	}
	launch_philo_batch(thinker, dinner.guest_count);
	usleep(500);
	launch_philo_batch(even_thinkers, dinner.guest_count);
}

void	set_table(int argc, char *args[])
{
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
		if (i == 0)
			dinner->guest_count = ft_atoi(args[i]);
		else if (i == 1)
			dinner->life_duration = ft_atoi(args[i]);
		else if (i == 2)
			dinner->meal_duration = ft_atoi(args[i]);
		else if (i == 3)
			dinner->sleep_duration = ft_atoi(args[i]);
		else if (i == 4)
			dinner->meals_required = ft_atoi(args[i]);
		i++;
		dinner->argc = argc;
	}
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
