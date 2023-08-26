/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleskine <rleskine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:41:38 by rleskine          #+#    #+#             */
/*   Updated: 2023/08/26 13:16:19 by rleskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <sys/time.h>
# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

# define PHILO_THINKING	0
# define PHILO_EATING	1
# define PHILO_SLEEPING	2
# define PHILO_SATED	3
# define PHILO_DEAD		4
# define PHILO_LFORK	5
# define PHILO_RFORK	6
# define PHILO_START	66
# define PHILO_ORDER66	66

# define PHILO_FINISH	1
# define CHECK_DEATH	4
# define EATING			4
# define CHECK_FINISH	3
# define CHECK_STOP		66
# define CHECK_START	66
# define CHK_STOP_START	66

# define LOGSIZE	5000

typedef struct s_logmsg {
	int		timestamp;
	int		philo;
	int		type;
	int		meal;
}	t_logmsg;

typedef struct s_log {
	t_logmsg	log[LOGSIZE * 2];
	int			lines;
	int			l;
	int			lines2;
	int			l2;
	int			i;
	int			sated;
	int			full;
}	t_log;

typedef struct s_brain {
	int					name;
	pthread_mutex_t		*left;
	pthread_mutex_t		*right;
	struct timeval		start;
	struct timeval		lastmeal;
	struct timeval		now;
	volatile int		meals;
	int					*times_to_eat;
	int					alive;
	pthread_mutex_t		*m_log;
	pthread_mutex_t		*m_die;
	pthread_mutex_t		*m_stop;
	int					stop;
	t_logmsg			*msg;
	t_log				*log;
	int					t_eat;
	int					t_slp;
	int					t_think;
}	t_brain;

typedef struct s_table {
	int					seats;
	int					times_to_eat;
	int					t_starve;
	pthread_t			superv;
	pthread_t			*phl;
	t_brain				*brn;
	pthread_mutex_t		*frk;
	pthread_mutex_t		m_die;
	pthread_mutex_t		m_log;
	pthread_mutex_t		m_stop;
}	t_table;

// philo_utils1.c

t_brain	*make_brain(int nbr, t_brain *b, pthread_mutex_t *f, int last);
int		getforks(t_brain *b, int err);
int		dropforks(t_brain *b);
void	stopall(t_table *t, int i);

// philo_utils2.c

int		read_args(char **ag, int n);
int		chk_args(int ac, char **ag);
int		checkmutex(t_brain *b, int status, struct timeval *time);
int		get_ms_diff(struct timeval *t1, struct timeval *t2, int now);
void	rsleep(int ms);

// log_utils1.c

t_log	*init_log(int philo);
int		add_log_msg(t_brain *b, int type, int meal);
int		nbr2str(int len, int nbr, char *s);

// log_utils2.c

void	print_msg(t_logmsg *msg, char *init);
void	choose_msg(t_logmsg *msg);
int		print_log(t_log *log, pthread_mutex_t *m_log);

#endif	/* PHILO_H */
