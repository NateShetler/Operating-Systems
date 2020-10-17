// Nathaniel Shetler
// Operating Systems
// 15 October 2020

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>
#include <time.h>

// Booleans used in the threads
bool isTobacco = false, isPaper = false, isMatch = false;

// Semaphores used for the threads
sem_t agentSem, tobacco, paper, matches;
sem_t tobaccoSem, paperSem, matchSem, mutex;

// Pusher thread prototypes
void *pusherTobacco();
void *pusherPaper();
void *pusherMatches();

// Agent thread prototypess
void *agent1();
void *agent2();
void *agent3();

// Smoker Thread prototypes
void *smokerTobacco();
void *smokerPaper();
void *smokerMatches();

int main()
{

    // Seed for random numbers that will be generated later (to simulate making cigarettes and smoking)
    srand(time(0));
    
    // The threads for lab 2
    pthread_t agent1Thread, agent2Thread, agent3Thread;
    pthread_t pusher1, pusher2, pusher3;
    pthread_t smokerTobacco1, smokerTobacco2, smokerPaper1, smokerPaper2, smokerMatches1, smokerMatches2;

    // Initialize the semaphores
    sem_init(&agentSem,0,1);
    sem_init(&tobacco,0,0);
    sem_init(&paper,0,0);
    sem_init(&matches,0,0);

    sem_init(&tobaccoSem,0,0);
    sem_init(&paperSem,0,0);
    sem_init(&matchSem,0,0);
    
    sem_init(&mutex,0,1); // mutex = 1

    // Create the threads
    pthread_create(&agent1Thread, NULL, agent1, NULL);
    pthread_create(&agent2Thread, NULL, agent2, NULL);
    pthread_create(&agent3Thread, NULL, agent3, NULL);

    pthread_create(&pusher1, NULL, pusherTobacco, NULL);
    pthread_create(&pusher2, NULL, pusherPaper, NULL);
    pthread_create(&pusher3, NULL, pusherMatches, NULL);

    pthread_create(&smokerTobacco1, NULL, smokerTobacco, NULL);
    pthread_create(&smokerTobacco2, NULL, smokerTobacco, NULL);
    pthread_create(&smokerPaper1, NULL, smokerPaper, NULL);
    pthread_create(&smokerPaper2, NULL, smokerPaper, NULL);
    pthread_create(&smokerMatches1, NULL, smokerMatches, NULL);
    pthread_create(&smokerMatches2, NULL, smokerMatches, NULL);

    // Join the threads
    pthread_join(agent1Thread, NULL);
    pthread_join(agent2Thread, NULL);
    pthread_join(agent3Thread, NULL);

    pthread_join(pusher1, NULL);
    pthread_join(pusher2, NULL);
    pthread_join(pusher3, NULL);

    pthread_join(smokerTobacco1, NULL);
    pthread_join(smokerTobacco2, NULL);
    pthread_join(smokerPaper1, NULL);
    pthread_join(smokerPaper2, NULL);
    pthread_join(smokerMatches1, NULL);
    pthread_join(smokerMatches2, NULL);

    return 0;
}

// This is the pusher thread for the tabacco
void *pusherTobacco()
{
    // Run the tobacco pusher 12 times before exiting the thread
    for (int i = 0; i < 12; ++i)
    {
        while (sem_wait(&tobacco) != 0) {}
        while (sem_wait(&mutex) != 0) {}
        if (isPaper == true)
        {
            isPaper = false;
            sem_post(&matchSem);
        }
        else if (isMatch == true)
        {
            isMatch = false;
            sem_post(&paperSem);
        }
        else
        {
            isTobacco = true;
        }

        sem_post(&mutex);
    }

    // Exit
    pthread_exit(0);
}

// This is the pusher thread for the paper
void *pusherPaper()
{
    // Run the paper pusher 12 times before exiting the thread
    for (int i = 0; i < 12; ++i)
    {
        while (sem_wait(&paper) != 0) {}
        while (sem_wait(&mutex) != 0) {}
        if (isTobacco == true)
        {
            isTobacco = false;
            sem_post(&matchSem);
        }
        else if (isMatch == true)
        {
            isMatch = false;
            sem_post(&tobaccoSem);
        }
        else
        {
            isPaper = true;
        }
        
        sem_post(&mutex);
    }

    // Exit
    pthread_exit(0);
}

// This is the pusher thread for the matches
void *pusherMatches()
{
    // Run the matches pusher 12 times before exiting the thread
    for (int i = 0; i < 12; ++i)
    {
        while (sem_wait(&matches) != 0) {}
        while (sem_wait(&mutex) != 0) {}
        if (isPaper == true)
        {
            isPaper = false;
            sem_post(&tobaccoSem);
        }
        else if (isTobacco == true)
        {
            isTobacco = false;
            sem_post(&paperSem);
        }
        else
        {
            isMatch = true;
        }

        sem_post(&mutex);
        
    }

    // Exit
    pthread_exit(0);
}

// This is the agent who deals the tobacco and paper
void *agent1()
{
    // Have the agent deal 6 times before exiting
    for (int i = 0; i < 6; i++)
    {
        while (sem_wait(&agentSem) != 0) {}
        sem_post(&tobacco);
        sem_post(&paper);
    }

    // Exit
    pthread_exit(0);
}

// This is the agent that deals tobacco and matches
void *agent2()
{
    // Have the agent deal 6 times before exiting
    for (int i = 0; i < 6; i++)
    {
        while (sem_wait(&agentSem) != 0) {}
        sem_post(&tobacco);
        sem_post(&matches);
    } 

    // Exit
    pthread_exit(0);
}

// This is the agent that deals matches and paper
void *agent3()
{
    // Have the agent deal 6 times before exiting
    for (int i = 0; i < 6; i++)
    {
        while (sem_wait(&agentSem) != 0) {}
        sem_post(&matches);
        sem_post(&paper);
    }

    // Exit
    pthread_exit(0);
}

// This is the smoker who has the tobacco
void *smokerTobacco()
{
    // Used for "making cigarette & smoking"
    int sleepTime;

    // The smoker will make 3 cigarettes and smoke them before exiting
    for (int i = 0; i < 3; i++)
    {
        // Generate random number up to 200 milliseconds
        sleepTime = rand()% 200000;

        while (sem_wait(&tobaccoSem) != 0) {}

        {
            // make cigarette
            printf("Smoker with tobacco making cigarette..\n");
            usleep(sleepTime);
        }

        sem_post(&agentSem);

        {
            // smoke the cigarette
            printf("Smoker with tobacco smoking cigarette..\n");
            usleep(sleepTime);
        }
    }
    
    // Indicate that the smoker is leaving
    printf("\n");
    printf("Smoking with tobacco is hungry and is leaving...\n");
    printf("\n");

    // Exit
    pthread_exit(0);
}

// This is the smoker who has the paper
void *smokerPaper()
{
    // Used for "making cigarette & smoking"
    int sleepTime;

    // The smoker will make 3 cigarettes and smoke them before exiting
    for (int i = 0; i < 3; i++)
    {
        // Generate random number up to 200 milliseconds
        sleepTime = rand()% 200000;

        // create random sleep time 
        while (sem_wait(&paperSem) != 0) {}

        {
            // make cigarette
            printf("Smoker with paper making cigarette..\n");
            usleep(sleepTime);
        }

        sem_post(&agentSem);

        {
            // smoke the cigarette
            printf("Smoker with paper smoking cigarette..\n");
            usleep(sleepTime);
        }
    }

    // Indicate that the smoker is leaving
    printf("\n");
    printf("Smoker with paper is hungry and is leaving...\n");
    printf("\n");

    // Exit
    pthread_exit(0);
}

// This is the smoker who has the matches
void *smokerMatches()
{
    // Used for "making cigarette & smoking"
    int sleepTime;

    // The smoker will make 3 cigarettes and smoke them before exiting
    for (int i = 0; i < 3; i++)
    {
        // Generate random number up to 200 milliseconds
        sleepTime = rand()% 200000;

        while (sem_wait(&matchSem) != 0) {}

        {
            // make cigarette
            printf("Smoker with matches making cigarette..\n");
            usleep(sleepTime);
        }

        sem_post(&agentSem);

        {
            // smoke the cigarette
            printf("Smoker with matches smoking cigarette..\n");
            usleep(sleepTime);
        }
    }

    // Indicate that the smoker is leaving
    printf("\n");
    printf("Smoker with matches is hungry and is leaving...\n");
    printf("\n");

    // Exit
    pthread_exit(0);
}