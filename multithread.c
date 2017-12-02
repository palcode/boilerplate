#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

#define BufferSize 10

#define DEBUG

void *Producer();
void *Consumer();


int BufferIndex=0;

char *Buffer;

pthread_cond_t Bfr_Not_Full=PTHREAD_COND_INITIALIZER;

pthread_cond_t Bfr_Not_Empty=PTHREAD_COND_INITIALIZER;

pthread_mutex_t mVar= PTHREAD_MUTEX_INITIALIZER;

int main()
{
pthread_t ptid,ctid;

Buffer = (char*) malloc (sizeof(char)*BufferSize);
pthread_create(&ptid,NULL,Producer,NULL);
pthread_create(&ctid,NULL,Consumer,NULL);

pthread_join(ptid,NULL);
pthread_join(ctid,NULL);

return 0;

}

void *Producer()
{

  for(;;)
  {
      pthread_mutex_lock(&mVar);
      if(BufferIndex==BufferSize)
       {
         pthread_cond_wait(&Bfr_Not_Full,&mVar); //So bascically if its become full, It will wait for other thread to signal that its not full
    
#ifdef DEBUG
printf("Test the values updated by consumer");
for(int i=0; i<BufferSize;i++)
printf(" %c ",Buffer[i]);

printf("\n");
#endif
       }
      
      Buffer[BufferIndex++]= '@';

     // printf("Produce: %d\n",BufferIndex);
      pthread_mutex_unlock(&mVar);
       
      if(BufferIndex==BufferSize)
      pthread_cond_signal(&Bfr_Not_Empty);

  }

}


void *Consumer()
{

  for(;;)
  {
      pthread_mutex_lock(&mVar);
      if(BufferIndex==-1)
       {
         pthread_cond_wait(&Bfr_Not_Empty,&mVar); //So bascically if its become empty, It will wait for other thread to signal that its not empty
#ifdef DEBUG
printf("Test the values updated by consumer");
for(int i=0; i<BufferSize;i++)
printf(" %c ",Buffer[i]);

printf("\n");
#endif

       }
      
      Buffer[BufferIndex--]= 'a';

     // printf("Consume: %d\n",BufferIndex);
      pthread_mutex_unlock(&mVar);
       
      if(BufferIndex==-1)
      pthread_cond_signal(&Bfr_Not_Full);

  }


}
