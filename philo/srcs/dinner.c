/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 10:01:54 by epinaud           #+#    #+#             */
/*   Updated: 2025/07/06 16:47:15 by epinaud          ###   ########.fr       */
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

static void	launch_philo_batch(t_guest *thinker, size_t batch_size)
{
	size_t	i;

	i = 0;
	if (thinker->id != 1)
		i++;
	while (i < batch_size)
	{
		thinker->last_meal = gset_dinner(0)->start_time;
		if (!pthread_create(&thinker->thread, NULL, launch_routine, thinker))
			thinker->status |= PHILO_THREAD;
		else
			put_err("Failled to create thread");
		thinker = thinker->next->next;
		i += 2;
	}
}

void	launch_dinner(t_guest *thinker, t_dinner dinner)
{
	t_guest	*even_thinkers;

	even_thinkers = thinker->next;
	gset_dinner(0)->start_time = time_since_epoch();
	if (dinner.guest_count == 1)
	{
		if (pthread_create(&thinker->thread, NULL, dine_alone, thinker) == 0)
			thinker->status |= PHILO_THREAD;
		else
			put_err("Failled to create thread");
		return ;
	}
	launch_philo_batch(thinker, dinner.guest_count);
	usleep((ft_min(dinner.life_duration, dinner.meal_duration) / 2) * 1000);
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
			ft_atoi(args[i], (int *)&dinner->guest_count);
		else if (i == 1)
			ft_atoi(args[i], (int *)&dinner->life_duration);
		else if (i == 2)
			ft_atoi(args[i], (int *)&dinner->meal_duration);
		else if (i == 3)
			ft_atoi(args[i], (int *)&dinner->sleep_duration);
		else if (i == 4)
			ft_atoi(args[i], (int *)&dinner->meals_required);
		i++;
		dinner->argc = argc;
	}
}

void	cleanup_table(t_dinner	*dinner, t_guest *philos)
{
	t_guest			*next_philo;
	size_t			i;

	i = 0;
	while (philos && i++ < dinner->guest_count)
	{
		if (philos->status & PHILO_THREAD)
			pthread_join(philos->thread, NULL);
		philos = philos->next;
	}
	i = 0;
	while (philos && i++ < dinner->guest_count)
	{
		next_philo = philos->next;
		if (philos->status & PHILO_MUTEX)
			pthread_mutex_destroy(&philos->fork_mutex);
		free(philos);
		philos = next_philo;
	}
	pthread_mutex_destroy(&gset_dinner(0)->coordinator);
	pthread_mutex_destroy(&gset_dinner(0)->display_lock);
}
