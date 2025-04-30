#ifndef PHILO_H
# define PHILO_H

#include <pthread.h>
#include <stdio.h>

typedef struct s_philosopher {
  int id;
  int left_fork;
  int right_fork;
  int eating_count;
  long  long  last_meal_time;//なぜlong long?
  pthread_mutex_t meal_mutex;
  struct  s_data *data;
  pthread_t thread;
} t_philosopher;

typedef struct s_fork {
  pthread_mutex_t mutex;
} t_fork;

typedef struct s_data {
  int n_philosopher;//哲学者の数
  int time_to_die;//死亡までの時間(ミリ秒)
  int time_to_eat;//食事にかかる時間(ミリ秒)
  int time_to_sleep;//睡眠にかかる時間(ミリ秒)
  int n_must_eat;//各哲学者が食べなければならない回数
  int simulation_end; //シュミレーション終了フラグ　フラグならboolの方が分かりやすくね？
  long  long  start_time;//シュミレーション
  t_fork *forks; //フォークの配列
  t_philosopher *philosophers;//哲学者の配列　　なぜダブルポインタで持たないのか
  pthread_mutex_t print_mutex;//出力を保護するミューテックス
  pthread_mutex_t end_mutex;//終了フラグを保護するミューテックス
  pthread_mutex_t meal_check_mutex;//食事回数チェックを保護するミューテックス
} t_data;

#endif
