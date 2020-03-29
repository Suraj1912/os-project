#include <pthread.h>

struct station {
  int station_passengers;
  int tratrain_passengers; 
  pthread_t lock;
  pthread_cond_t train_arrived_cond;
  pthread_cond_t passengers_seated_cond;
  pthread_cond_t train_is_full_cond;
};

void station_init(struct station *station);
{
	  station->station_passengers = 0;
	  station->train_passengers = 0;
	  pthread_mutex_init(&(station->lock), NULL);
	  pthread_cond_init(&(station->train_arrived_cond), NULL);
	  pthread_cond_init(&(station->passengers_seated_cond), NULL);
	  pthread_cond_init(&(station->train_is_full_cond), NULL);
}

void station_load_train(struct station *station, int count);
{
	pthread_mutex_lock(&(station->lock));

  while ((station->station_passengers > 0) && (count > 0)){
    pthread_cond_signal(&(station->train_arrived_cond));
  	count--;
  	pthread_cond_wait(&(station->passengers_seated_cond), &(station->lock));
  }

  if (station->train_passengers > 0)
  	pthread_cond_wait(&(station->train_is_full_cond), &(station->lock));

 
  pthread_mutex_unlock(&(station->lock));
}

void station_wait_for_train(struct station *station);
{
	  pthread_mutex_lock(&(station->lock));

  station->station_passengers++;
  pthread_cond_wait(&(station->train_arrived_cond), &(station->lock));
  station->station_passengers--;
  station->train_passengers++;

  pthread_mutex_unlock(&(station->lock));

  pthread_cond_signal(&(station->passengers_seated_cond));
}

void station_on_board(struct station *station);
{
	 pthread_mutex_lock(&(station->lock));

  station->train_passengers--;

  pthread_mutex_unlock(&(station->lock));

  if (station->train_passengers == 0)
  	pthread_cond_broadcast(&(station->train_is_full_cond));
}
