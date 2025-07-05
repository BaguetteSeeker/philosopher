/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 21:25:35 by epinaud           #+#    #+#             */
/*   Updated: 2025/07/05 21:23:01 by epinaud          ###   ########.fr       */
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

static void	create_philos(t_guest **head, t_dinner dinner)
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
		if (pthread_mutex_init(&thinker->fork_mutex, NULL) == 0)
			thinker->status |= PHILO_MUTEX;
		else
			put_err("Failled to initialize mutex");
		ft_lstadd_back(head, thinker);
		i++;
	}
	thinker->next = *head;
	dinner.philos = *head;
}

int	main(int argc, char *argv[])
{
	t_dinner		dinner;

	dinner = (t_dinner){0};
	gset_dinner(&dinner);
	if (argc < 5)
		put_err("Insufficient argument count");
	set_table(argc, &argv[1]);
	if (dinner.guest_count == 0)
		put_err("There are no guest to your table..");
	create_philos(&dinner.philos, dinner);
	launch_dinner(dinner.philos, dinner);
	cleanup_table(&dinner, dinner.philos);
}
