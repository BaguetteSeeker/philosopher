/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:19:30 by epinaud           #+#    #+#             */
/*   Updated: 2025/07/02 18:40:05 by epinaud          ###   ########.fr       */
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

static void	lift_forks(t_guest *philo)
{
	if (is_dinner_done())
		return ;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->fork_mutex);
		display_state(philo, PICKING_FORK);
		check_death(philo);
		if (is_dinner_done())
			return ;
		pthread_mutex_lock(&philo->next->fork_mutex);
		display_state(philo, PICKING_FORK);
	}
	else
	{
		pthread_mutex_lock(&philo->next->fork_mutex);
		display_state(philo, PICKING_FORK);
		check_death(philo);
		if (is_dinner_done())
			return ;
		pthread_mutex_lock(&philo->fork_mutex);
		display_state(philo, PICKING_FORK);
	}
}

void	check_death(t_guest *philo)
{
	t_dinner				*table;

	table = gset_dinner(0);
	if (table->life_duration == 0 || time_since_epoch()
		- philo->last_meal > table->life_duration)
	{
		pthread_mutex_lock(&table->coordinator);
		if (table->is_done)
		{
			pthread_mutex_unlock(&table->coordinator);
			return ;
		}
		table->is_done = true;
		pthread_mutex_unlock(&table->coordinator);
		display_state(philo, DIED);
		return ;
	}
}

static size_t	eat(t_guest *philo)
{
	t_dinner	*table;

	table = gset_dinner(0);
	lift_forks(philo);
	philo->last_meal = time_since_epoch();
	philo->times_eaten++;
	display_state(philo, EATING);
	ft_usleep(table->meal_duration, philo, EATING);
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(&philo->fork_mutex);
		pthread_mutex_unlock(&philo->next->fork_mutex);
		// printf("Philo %ld realising forks at %ld\n", philo->id, time_since_start());
	}
	else
	{
		pthread_mutex_unlock(&philo->next->fork_mutex);
		pthread_mutex_unlock(&philo->fork_mutex);
		// printf("Philo %ld realising forks at %ld\n", philo->id, time_since_start());
	}
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
		ft_usleep(table->sleep_duration, philo, SLEEPING);
		check_death(philo);
		if (is_dinner_done())
			break ;
		display_state(philo, THINKING);
		check_death(philo);
		if (is_dinner_done())
			break ;
	}
	return (NULL);
}
