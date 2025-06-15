/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putlogs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 18:37:41 by epinaud           #+#    #+#             */
/*   Updated: 2025/06/15 18:42:59 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	display_state(t_philosopher *philo, size_t action)
{
	if (action == EATING)
		printf("Philosopher number %ld is eating\n", philo->number);
	else if (action == THINKING)
		printf("Philosopher number %ld is thinking\n", philo->number);
	else if (action == SLEEPING)
		printf("Philosopher number %ld is sleeping\n", philo->number);
}