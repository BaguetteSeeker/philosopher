/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:19:30 by epinaud           #+#    #+#             */
/*   Updated: 2025/06/27 19:19:05 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*dine_alone(void *philo)
{
	ft_usleep(gset_dinner(0)->life_duration / 0.001, (t_guest *)philo);
	display_state((t_guest *)philo, DIED);
	return (NULL);
}

void	lift_forks(t_guest *philo)
{
	if (is_dinner_done())
		return ;
	if (&philo->fork_mutex < &philo->next->fork_mutex)
	{
		pthread_mutex_lock(&philo->fork_mutex);
		display_state(philo, PICKING_FORK);
		pthread_mutex_lock(&philo->next->fork_mutex);
		display_state(philo, PICKING_FORK);
	}
	else
	{
		pthread_mutex_lock(&philo->next->fork_mutex);
		display_state(philo, PICKING_FORK);
		pthread_mutex_lock(&philo->fork_mutex);
		display_state(philo, PICKING_FORK);
	}
}

void	check_death(t_guest *philo)
{
	t_dinner	*table;
	static int	death = 0;

	table = gset_dinner(0);
	if (table->life_duration == 0
	|| time_since_epoch() - philo->last_meal > table->life_duration)
	{
		if (death)
			return ;
		else
			death = 1;
		pthread_mutex_lock(&table->coordinator);
		table->is_done = true;
		pthread_mutex_unlock(&table->coordinator);
		display_state(philo, DIED);
		return ;
	}
}

size_t	eat(t_guest *philo)
{
	t_dinner	*table;

	table = gset_dinner(0);
	lift_forks(philo);
	display_state(philo, EATING);
	ft_usleep(table->meal_duration, philo);
	philo->times_eaten++;
	philo->last_meal = time_since_epoch();
	pthread_mutex_unlock(&philo->fork_mutex);
	pthread_mutex_unlock(&philo->next->fork_mutex);
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
		if (is_dinner_done() || !eat(philo) || is_dinner_done())
			break ;
		display_state(philo, SLEEPING);
		ft_usleep(table->sleep_duration, philo);
		if (is_dinner_done())
			break ;
		display_state(philo, THINKING);
		if (is_dinner_done())
			break ;
	}
	return (NULL);
}
