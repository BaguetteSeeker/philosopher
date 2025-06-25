/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 21:25:35 by epinaud           #+#    #+#             */
/*   Updated: 2025/06/25 18:56:33 by epinaud          ###   ########.fr       */
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

t_guest	*create_philos(t_guest **head, t_dinner dinner)
{
	t_guest			*thinker;
	size_t			i;

	i = 1;
	while (i < dinner.guest_count + 1)
	{
		thinker = malloc(sizeof(t_guest));
		if (!thinker)
			put_err("Failled to malloc philosopher");
		*thinker = (t_guest){0};
		thinker->id = i;
		if (pthread_mutex_init(&thinker->fork_mutex, NULL) != 0)
			put_err("Failled to initialize philosopher");
		ft_lstadd_back(head, thinker);
		i++;
	}
	thinker->next = *head;
	return (*head);
}

void	init_philo(t_guest **head, t_dinner dinner)
{
	t_guest			*thinker;
	size_t			i;

	thinker = create_philos(head, dinner);
	gset_dinner(0)->start_time = time_since_epoch();
	launch_dinner(*head, dinner);
	i = 0;
	while (i++ < dinner.guest_count)
	{
		pthread_join(thinker->thread, NULL);
		thinker = thinker->next;
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
	/* printf("<<The following message should never appear before any philo log\
>>\nBOTH THREADS HAVE RETURNED\n Main thread unshackled\n"); */
	cleanup_table(&dinner, dinner.philos);
	pthread_mutex_destroy(&gset_dinner(0)->coordinator);
}
