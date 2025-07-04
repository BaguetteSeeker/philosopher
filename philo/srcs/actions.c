/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:19:30 by epinaud           #+#    #+#             */
/*   Updated: 2025/07/04 22:00:56 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*dine_alone(void *philo)
{
	pthread_mutex_lock(&((t_guest *)philo)->fork_mutex);
	display_state(philo, PICKING_FORK);
	usleep(gset_dinner(0)->life_duration / 0.001);
	display_state((t_guest *)philo, DIED);
	pthread_mutex_unlock(&((t_guest *)philo)->fork_mutex);
	return (NULL);
}

void	think(t_guest *philo, bool silent)
{
	long	time_to_think;

	time_to_think = (gset_dinner(0)->life_duration
			- (time_since_epoch() - philo->last_meal)) / 2;
	printf("%ld Time to think for philo %ld is %ld\n", time_since_start(), philo->id, time_to_think);
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && silent == true)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (silent == false)
	display_state(philo, THINKING);
	ft_usleep(time_to_think, philo);
}

static bool	lift_forks(t_guest *philo)
{
	pthread_mutex_t	*st_fork;
	pthread_mutex_t	*nd_fork;

	if (philo->id % 2 == 0)
	{
		st_fork = &philo->fork_mutex;
		nd_fork = &philo->next->fork_mutex;
	}
	else
	{
		st_fork = &philo->next->fork_mutex;
		nd_fork = &philo->fork_mutex;
	}
	if (is_dinner_done())
		return (0);
	if (dies_with_fork(philo, NULL))
		return (0);
	pthread_mutex_lock(st_fork);
	display_state(philo, PICKING_FORK);
	if (dies_with_fork(philo, st_fork))
		return (0);
	pthread_mutex_lock(nd_fork);
	display_state(philo, PICKING_FORK);
	return (1);
}

static size_t	eat(t_guest *philo)
{
	t_dinner		*table;
	pthread_mutex_t	*st_fork;
	pthread_mutex_t	*nd_fork;

	table = gset_dinner(0);
	if (philo->id % 2 == 0)
	{
		st_fork = &philo->fork_mutex;
		nd_fork = &philo->next->fork_mutex;
	}
	else
	{
		st_fork = &philo->next->fork_mutex;
		nd_fork = &philo->fork_mutex;
	}
	if (!lift_forks(philo) || is_dinner_done())
		return (0);
	philo->last_meal = time_since_epoch();
	philo->times_eaten++;
	display_state(philo, EATING);
	ft_usleep(table->meal_duration, philo);
	pthread_mutex_unlock(st_fork);
	pthread_mutex_unlock(nd_fork);
	return (1);
}

void	*launch_routine(void *v_philo)
{
	t_guest			*philo;
	t_dinner		*table;

	table = gset_dinner(0);
	philo = (t_guest *)v_philo;
	while (1)
	{
		if (table->argc == MAX_ARGS
			&& philo->times_eaten >= table->meals_required)
			break ;
		check_death(philo);
		if (is_dinner_done() || !eat(philo) || is_dinner_done())
			break ;
		display_state(philo, SLEEPING);
		ft_usleep(table->sleep_duration, philo);
		check_death(philo);
		if (is_dinner_done())
			break ;
		think(philo, false);
		check_death(philo);
		if (is_dinner_done())
			break ;
	}
	return (NULL);
}
