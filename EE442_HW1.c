#include <pthread.h>
#include "Queue.c"
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
//typedef int semaphore;  //semaphores are a special kind of int
//semaphore s=0;		//functions initial variable.
#define true 1
typedef struct Semaphore{		// this semaphore will provide us to synch. the threads.

	int mutex;
	int empty;
	int full;

}Semaphore;
pthread_mutex_t lockGen;
pthread_mutex_t lockPro;
typedef struct ParametersToEnterGenerator
{

int AmountofRandomNumbers_;
int MaxRange;
Queue *queue;
Semaphore *semaphore_;
int Tid;
}ParametersToEnterGenerator;

// Creation of atomic functions for critical Section among the threads
void down(int *s)	//wait if the mutex number s less than or equal to  0
{
while (*s==0);
*s=*s-1;
}



void up(int *s){ //increase s if the process is proceed.
*s=*s+1;
}

void *RandomNumberGenerator(void *context)
{
ParametersToEnterGenerator *parameter = context;
int Maxrange = parameter->MaxRange;
int AmountOfRandomNumbers = parameter->AmountofRandomNumbers_;
Queue *DummyQueue=parameter->queue;
int Empty = parameter->semaphore_->empty;
int Mutex = parameter->semaphore_->mutex;
int Full  = parameter->semaphore_->full;
int item=0;
int Counter=0;
 
while(true){
    	  if (Counter <= AmountOfRandomNumbers){	//

			item=rand() % ((Maxrange)+1);   // generate something to put in buffer in the range.Generates number with a repatitive order.
			pthread_mutex_lock(&lockGen);
			//             down(&Empty);                   //enter critical region. When we decrease the mutex to 0, no thread can enter to its $
                        QueueInsert(DummyQueue,item);            //inser the number to the queue
                        Counter+=1;                     //increase the counter since it wll not enter any more to the function when the amoun$
                  //      printf("the item is= %d", item);
                  //      up(&Mutex);                     //increse the mutual exclusion function when we have done with the function to make a$
             //           up(&Full);                      //this variable will be used for other consumer threads(the threads that will use the$
			pthread_mutex_unlock(&lockGen); 
	 }
	  else
	  {
                // exit the thread if the specified number is reached...
			 printf("Random number generation is ended...\n");
              		 pthread_exit(&AmountOfRandomNumbers);
          }
}
}


void *DividerFindingThread (void *context)
{

ParametersToEnterGenerator *Parameters = context;
Queue *DummyQ=Parameters->queue;
int Number=0;
int MaxArraySize = Parameters->MaxRange;
int Divisors[MaxArraySize];
int count=0;
int empty=Parameters->semaphore_->empty;
int full = Parameters->semaphore_->full;
int counter=0;
float Lower=0.1;
int Upper =1;
float Step= 0.1;

//float random_value = (rand() % ((++Upper - Lower) / step)) * step + Lower;


while(true){
		if (DummyQ->size ==0)
			break;
		else
		{
	//		down(&full);
                        pthread_mutex_lock(&lockGen);	//Lock thread so that no other threads can enter critical section of producer.
			pthread_mutex_lock(&lockPro);	// mutex for consumer threads not to fetch data from the Q at the same time .
			//while(DummyQ->size==0);		// if there is no item in the queue wait until a number is appear.

			Number= QueueRemove(DummyQ);            //inser the number to the queue
			pthread_mutex_unlock(&lockPro);
			pthread_mutex_unlock(&lockGen); 
          //      	up(&empty);
			for(int i=1;i<=Number;i++){		//Finding the divisors of the Number.
				if(Number%i==0){
					Divisors[count]=i;
					count++;
					}
			}
			printf("Thread id=%lu, Number: %d, Divisors:  ",pthread_self() ,Number );
			int DummyVariable=count+1;
			while(count>0)
			{
				printf("%d  ", Divisors[DummyVariable-count-1]);  // dummyvariable is to write the array starting from 0.1.2..
				count--;
			}
			printf("\n");
			count=0;
			for (int i=0; i<=count;i++)
				Divisors[i]=0;
			float random_value = rand() %9 + 0.1;
			sleep(random_value);
			//sleep(0.2);

		}

}
}


int main(int argc, char *argv[])
{

int NumberOfWorkerThreads=5;		//maximum size of threads that will work on the generated numbers.Defauls:5
int MaxSizeOfQueue=10;     		//Maximum size of Queue, which is shared by threads.Default :10
int AmountOfRandomNumbers=20;		//The amount of the numbers that random number generator function will generate. Default:20
int RandomNumberRange =100;		//The possible range of the random numbers.Default :100

pthread_mutex_init(&lockGen, NULL);
pthread_mutex_init(&lockPro, NULL);
int option = 0;
while ((option = getopt(argc, argv,"t:q:r:m:")) != -1) {	// parsing the terminal command window.
        switch (option) {
             case 't' : NumberOfWorkerThreads = atoi(optarg);
                 break;
             case 'q' : MaxSizeOfQueue = atoi(optarg);
                 break;
             case 'r' : AmountOfRandomNumbers = atoi(optarg);
                 break;
             case 'm' : RandomNumberRange = atoi(optarg);
                 break;
             default:
               //  printf("No argument parameter is entered. Default Values will be used...");
               abort();
	}

}


Queue *Q = QueueInitialize(MaxSizeOfQueue); // this value will be parsed from the terminal and will be decided by -q parse variable.
Semaphore semaphore ={.mutex=1,
		      .empty=MaxSizeOfQueue,
		      .full=0};	// initialize the semaphore.
//Semaphore *semaphore = malloc (sizeof(Struct Semaphore));

ParametersToEnterGenerator GeneratorParameters={.AmountofRandomNumbers_=AmountOfRandomNumbers,
						.MaxRange=RandomNumberRange,
						.queue=Q,
						.semaphore_=&semaphore,
						.Tid=0};


int GeneratorSuccess;		// parameter to understand if the Generator thread is created
int WorkerSuccess;		// parameter to understand if the Producer thread is created
pthread_t GeneratorThread;	//thread definations
pthread_t WorkerThreads[NumberOfWorkerThreads];


GeneratorSuccess = pthread_create(&GeneratorThread,NULL,RandomNumberGenerator,(void *) &GeneratorParameters); // generator thread creation
if(GeneratorSuccess){
        printf("Thread generation FAILED!\n");
}


for (int CurrentThreadNumber=0; CurrentThreadNumber<NumberOfWorkerThreads; CurrentThreadNumber++ ) // worker threads creation
{
	
		WorkerSuccess = pthread_create(&WorkerThreads[CurrentThreadNumber],NULL,DividerFindingThread,(void *) &GeneratorParameters);
		if(WorkerSuccess){
        		printf("Thread generation FAILED!\n");
		}
}

pthread_join(GeneratorThread, NULL);

for (int i=0; i<NumberOfWorkerThreads;  i++){
	pthread_join(WorkerThreads[i], NULL);
}

return 0;


}

