/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 21:25:35 by epinaud           #+#    #+#             */
/*   Updated: 2025/06/20 00:54:23 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_dinner	*gset_dinner(void *g)
{
	static t_dinner	*stored_g = {NULL};

	if (!stored_g)
		stored_g = g;
	return (stored_g);
}

//Deal with atoi 0 return edge case
int	set_table(int argc, char *args[])
{
	size_t		val;
	size_t		i;
	t_dinner	*dinner;

	i = 0;
	dinner = gset_dinner(0);
	while (args[i])
	{
		val = ft_atoi(args[i]);
		if (ft_strlen(args[i]) != ft_nbrlen(val))
			put_err("");
		if (i == 0)
			dinner->guest_count = val;
		else if (i == 1)
			dinner->life_duration = val;
		else if (i == 2)
			dinner->meal_duration = val / 0.001;
		else if (i == 3)
			dinner->sleep_duration = val / 0.001;
		else if (i == 4)
			dinner->meals_required = val;
		i++;
		dinner->argc = argc;
	}
	return (1);
}

t_philosopher	*create_philos(t_philosopher **head, t_dinner dinner)
{
	size_t			i;
	t_philosopher	*thinker;

	i = 1;
	while (i < dinner.guest_count + 1)
	{
		thinker = malloc(sizeof(t_philosopher));
		if (!thinker)
			put_err("Failled to malloc philosopher");
		*thinker = (t_philosopher){0};
		thinker->id = i;
		if (pthread_mutex_init(&thinker->fork_mutex, NULL) != 0)
			put_err("Failled to initialize philosopher");
		ft_lstadd_back((t_list **)head, (t_list *)thinker);
		i++;
	}
	thinker->next = *head;
	return (*head);
}

void	launch_philos(t_philosopher *thinker, t_dinner dinner)
{
	size_t	i;
	
	if (dinner.guest_count == 1)
	{
		usleep(dinner.life_duration / 0.001);
		display_state(thinker, DIED);
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


//DEATH 0 ??
//Thread the standalone philo case
void	init_philo(t_philosopher **head, t_dinner dinner)
{
	size_t			i;
	t_philosopher	*thinker;

	thinker = create_philos(head, dinner);
	gset_dinner(0)->start_time = time_since_epoch();
	// printf("Dinner start at %ld\n Sec since start %ld\n",
	// 	gset_dinner(0)->start_time, time_since_epoch() - gset_dinner(0)->start_time);
	launch_philos(*head, dinner);
	while (i++ < dinner.guest_count)
	{
		pthread_join(thinker->thread, NULL);
		thinker = thinker->next;
	}
	printf("Exited philo init\n");
}

void	cleanup_philos(t_dinner	*dinner, t_philosopher *philos)
{
	size_t			i;
	t_philosopher	*next_philo;

	i = 0;
	while (i++ < dinner->guest_count)
	{
		next_philo = philos->next;
		pthread_mutex_destroy(&philos->fork_mutex);
		free(philos);
		philos = next_philo;
	}
}

int	main(int argc, char *argv[])
{
	t_dinner		dinner;

	dinner = (t_dinner){0};
	gset_dinner(&dinner);
	if (argc < 5)
		put_err("Insufficient argument count");
	set_table(argc, &argv[1]);
	printf("There is %ld philosophers around the table\n", dinner.guest_count);
	init_philo(&dinner.philos, dinner);
	printf("<<The following message should never appear before any philo log\
>>\nBOTH THREADS HAVE RETURNED\n Main thread unshackled\n");
	cleanup_philos(&dinner, dinner.philos);
}
