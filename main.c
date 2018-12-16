#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct buffer
{
    int tab[10]; //buffer of elements
    int count;
    sem_t mutex;
    sem_t empty;
    sem_t full;
}buffer;

buffer *(array_of_buffers[5]);
sem_t mutex2;

void insert_item(int to_insert, buffer *buff)
{
  buff->tab[buff->count]=to_insert;
  ++buff->count;
};

int erase_item(buffer *buff)
{
  int to_return=buff->tab[0];
  int i=0;
  for(i=0; i<buff->count-1; i++)
    buff->tab[i]=buff->tab[i+1];
  --buff->count;
  return to_return;
}

void print1(int number, buffer *buff)
{
  int i=0;
  printf("\n\nKolejka %d: dodano element %d\n", number, buff->tab[buff->count-1]);
  printf("Stan kolejki: ");
  for(i=0; i<buff->count; i++)
    printf("%d ", buff->tab[i]);
}

void print2(int number, int erased, buffer *buff)
{
  int i=0;
  printf("\n\nKolejka %d: usunieto element %d\n", number, erased);
  printf("Stan kolejki: ");
  for(i=0; i<buff->count; i++)
    printf("%d ", buff->tab[i]);
}

void producer()
{
  int number, new_element;

  number=rand()%5;
  new_element=rand()%100;

  sem_wait(&array_of_buffers[number]->empty);
  sem_wait(&array_of_buffers[number]->mutex);
  insert_item(new_element, array_of_buffers[number]);
  sem_wait(&mutex2);
  print1(number+1, array_of_buffers[number]);
  sem_post(&mutex2);
  sem_post(&array_of_buffers[number]->mutex);
  sem_post(&array_of_buffers[number]->full);
}

void consumer1()
{
  int erased=0;
  sem_wait(&array_of_buffers[0]->full);
  sem_wait(&array_of_buffers[0]->mutex);
  erased=erase_item(array_of_buffers[0]);
  sem_wait(&mutex2);
  print2(1, erased, array_of_buffers[0]);
  sem_post(&mutex2);
  sem_post(&array_of_buffers[0]->mutex);
  sem_post(&array_of_buffers[0]->empty);
}

void consumer2()
{
  int erased=0;
  sem_wait(&array_of_buffers[1]->full);
  sem_wait(&array_of_buffers[1]->mutex);
  erased=erase_item(array_of_buffers[1]);
  sem_wait(&mutex2);
  print2(2, erased, array_of_buffers[1]);
  sem_post(&mutex2);
  sem_post(&array_of_buffers[1]->mutex);
  sem_post(&array_of_buffers[1]->empty);
}

void consumer3()
{
  int erased=0;
  sem_wait(&array_of_buffers[2]->full);
  sem_wait(&array_of_buffers[2]->mutex);
  erased=erase_item(array_of_buffers[2]);
  sem_wait(&mutex2);
  print2(3, erased, array_of_buffers[2]);
  sem_post(&mutex2);
  sem_post(&array_of_buffers[2]->mutex);
  sem_post(&array_of_buffers[2]->empty);
}

void consumer4()
{
  int erased=0;
  sem_wait(&array_of_buffers[3]->full);
  sem_wait(&array_of_buffers[3]->mutex);
  erased=erase_item(array_of_buffers[3]);
  sem_wait(&mutex2);
  print2(4, erased, array_of_buffers[3]);
  sem_post(&mutex2);
  sem_post(&array_of_buffers[3]->mutex);
  sem_post(&array_of_buffers[3]->empty);
}

void consumer5()
{
  int erased=0;
  sem_wait(&array_of_buffers[4]->full);
  sem_wait(&array_of_buffers[4]->mutex);
  sem_wait(&mutex2);
  erased=erase_item(array_of_buffers[4]);
  print2(5, erased, array_of_buffers[4]);
  sem_post(&mutex2);
  sem_post(&array_of_buffers[4]->mutex);
  sem_post(&array_of_buffers[4]->empty);
}

void perform_test1()
{
  while(1)
  {
    pthread_t new_thread;
    int number=rand()%5;
    usleep(20000);

    pthread_create(&new_thread, NULL, (void *) producer, NULL);

    if (number == 0) pthread_create(&new_thread, NULL, (void *) consumer1, NULL);
    else if (number == 1) pthread_create(&new_thread, NULL, (void *) consumer2, NULL);
    else if (number == 2) pthread_create(&new_thread, NULL, (void *) consumer3, NULL);
    else if (number == 3) pthread_create(&new_thread, NULL, (void *) consumer4, NULL);
    else if (number == 4) pthread_create(&new_thread, NULL, (void *) consumer5, NULL);
    else break;
  }
}

void perform_test2()
{
  while(1)
  {
    int i;
    pthread_t new_thread;
    int number=rand()%5;
    usleep(20000);

    for(i = 0; i < 2; i++)
      pthread_create(&new_thread, NULL, (void *) producer, NULL);

    if (number == 0) pthread_create(&new_thread, NULL, (void *) consumer1, NULL);
    else if (number == 1) pthread_create(&new_thread, NULL, (void *) consumer2, NULL);
    else if (number == 2) pthread_create(&new_thread, NULL, (void *) consumer3, NULL);
    else if (number == 3) pthread_create(&new_thread, NULL, (void *) consumer4, NULL);
    else if (number == 4) pthread_create(&new_thread, NULL, (void *) consumer5, NULL);
    else break;
  }
}

void perform_test3()
{
  while(1)
  {
    pthread_t new_thread;
    int number = rand() % 5;
    usleep(20000);

    pthread_create(&new_thread, NULL, (void *) producer, NULL);

    if (number == 0)
    {
      int i=0;
      for(i = 0; i < 2; i++)
        pthread_create(&new_thread, NULL, (void *) consumer1, NULL);
    }
    else if (number == 1)
    {
      int i=0;
      for(i = 0; i < 2; i++)
        pthread_create(&new_thread, NULL, (void *) consumer2, NULL);
    }
    else if (number == 2)
    {
      int i=0;
      for(i = 0; i < 2; i++)
        pthread_create(&new_thread, NULL, (void *) consumer3, NULL);
    }
    else if (number == 3)
    {
      int i=0;
      for(i = 0; i < 2; i++)
        pthread_create(&new_thread, NULL, (void *) consumer4, NULL);
    }
    else if (number == 4)
    {
      int i=0;
      for(i = 0; i < 2; i++)
        pthread_create(&new_thread, NULL, (void *) consumer5, NULL);
    }
    else break;
  }
}

int main()
{
  unsigned seed;
  time_t t;
  int i, j;
  char c;

  for(i = 0; i < 5; i++)
    array_of_buffers[i]=(buffer *)malloc(sizeof(buffer));

  seed=time(&t);
  srand(seed);

  for(j = 0; j < 5; j++)
  {
    sem_init(&array_of_buffers[j]->mutex, 0, 1);
    sem_init(&array_of_buffers[j]->empty, 0, 10);
    sem_init(&array_of_buffers[j]->full, 0, 0);
  }

  sem_init(&mutex2, 0, 1);

  printf("Wybierz test:\n1-test zwykly\n2-test z nadmiarowym producentem\n3-test z nadmiarowymi konsumentami\n");
  c=getchar();
  if(c=='1') perform_test1();
  else if (c=='2') perform_test2();
  else if(c=='3') perform_test3();

  for(j = 0; j < 5; j++)
  {
    sem_destroy(&array_of_buffers[i]->mutex);
    sem_destroy(&array_of_buffers[i]->empty);
    sem_destroy(&array_of_buffers[i]->full);
  }

  sem_destroy(&mutex2);

  for(i = 0; i < 5; i++)
    free(array_of_buffers[i]);

  return 0;
}