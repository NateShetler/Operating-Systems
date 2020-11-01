// Nathaniel Shetler
// Email: nds39@zips.uakron.edu
// ID: 4015423
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
void *pusherTobacco(); // Pusher function/thread for tobacco
void *pusherPaper(); // Pusher function/thread for paper
void *pusherMatches(); // Pusher function/thread for matches

// Agent thread prototypess
void *agent1(); // Function/thread of the agent who deals the tobacco and paper
void *agent2(); // Function/thread of the agent that deals tobacco and matches
void *agent3(); // Function/thread of the agent that deals matches and paper

// Smoker Thread prototypes
void *smokerTobacco(); // Function/thread of the smoker that has tobacco
void *smokerPaper(); // Function/thread of the smoker that has paper
void *smokerMatches(); // Function/thread of the smoker that has matches

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
        while (sem_wait(&tobacco) != 0) {} // 'P' (wait) for tobacco semaphore
        while (sem_wait(&mutex) != 0) {} // 'P' (wait) for mutex semaphore
        if (isPaper == true) // If it is paper
        {
            isPaper = false;
            sem_post(&matchSem); // 'V' for matchSem semaphore
        }
        else if (isMatch == true) // If it is match
        {
            isMatch = false;
            sem_post(&paperSem); // 'V' for paperSem semaphore
        }
        else
        {
            // Otherwise, set isTobacco to true
            isTobacco = true;
        }

        // 'V' for mutex semaphore
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
        while (sem_wait(&paper) != 0) {} // 'P' (wait) for paper semaphore
        while (sem_wait(&mutex) != 0) {} // 'P' (wait) for mutex semaphore
        if (isTobacco == true) // If it is tobacco
        {
            isTobacco = false;
            sem_post(&matchSem); // 'V' for matchSem semaphore
        }
        else if (isMatch == true) // If it is match
        {
            isMatch = false;
            sem_post(&tobaccoSem); // 'V' for tobaccoSem semaphore
        }
        else
        {
            // Otherwise, set isPaper to true
            isPaper = true;
        }
        
        // 'V' for mutex semaphore
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
        while (sem_wait(&matches) != 0) {} // 'P' (wait) for matches semaphore
        while (sem_wait(&mutex) != 0) {} // 'P' (wait) for mutex semaphore
        if (isPaper == true) // If it is paper
        {
            isPaper = false;
            sem_post(&tobaccoSem); // 'V' for tobaccoSem semaphore
        }
        else if (isTobacco == true) // If it is tobacco
        {
            isTobacco = false;
            sem_post(&paperSem); // 'V' for paperSem semaphore
        }
        else
        {
            // Otherwise, set isMatch to true
            isMatch = true;
        }

        // 'V' for mutex semaphore
        sem_post(&mutex);
        
    }

    // Exit
    pthread_exit(0);
}

// This is the agent who deals the tobacco and paper
void *agent1()
{
    // Used for mixing up order
    int sleepTime;

    // Have the agent deal 6 times before exiting
    for (int i = 0; i < 6; i++)
    {
        // Generate random number up to 200 milliseconds
        sleepTime = rand()% 200000;
        usleep(sleepTime);

        while (sem_wait(&agentSem) != 0) {} // 'P' (wait) for agentSem semaphore
        sem_post(&tobacco); // 'V' for tobacco semaphore
        sem_post(&paper); // 'V' for tobacco semaphore
    }

    // Exit
    pthread_exit(0);
}

// This is the agent that deals tobacco and matches
void *agent2()
{
    // Used for mixing up order
    int sleepTime;

    // Have the agent deal 6 times before exiting
    for (int i = 0; i < 6; i++)
    {
        // Generate random number up to 200 milliseconds
        sleepTime = rand()% 200000;
        usleep(sleepTime);

        while (sem_wait(&agentSem) != 0) {} // 'P' (wait) for agentSem semaphore
        sem_post(&tobacco); // 'V' for tobacco semaphore
        sem_post(&matches); // 'V' for matches semaphore
    } 

    // Exit
    pthread_exit(0);
}

// This is the agent that deals matches and paper
void *agent3()
{
    // Used for mixing up order
    int sleepTime;

    // Have the agent deal 6 times before exiting
    for (int i = 0; i < 6; i++)
    {
        // Generate random number up to 200 milliseconds
        sleepTime = rand()% 200000;
        usleep(sleepTime);

        while (sem_wait(&agentSem) != 0) {} // 'P' (wait) for agentSem semaphore
        sem_post(&matches); // 'V' for matches semaphore
        sem_post(&paper); // 'V' for paper semaphore
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
        while (sem_wait(&tobaccoSem) != 0) {} // 'P' (wait) for tobaccoSem semaphore

        {
            // make cigarette
            printf("Smoker with tobacco is making a cigarette..\n");

            // Generate random number up to 50 milliseconds
            sleepTime = rand()% 50000;
            usleep(sleepTime);
        }

        sem_post(&agentSem); // 'V' agentSem semaphore

        {
            // smoke the cigarette
            printf("Smoker with tobacco is smoking a cigarette..\n");

            // Generate random number up to 50 milliseconds
            sleepTime = rand()% 50000;
            usleep(sleepTime);
        }
    }
    
    // Indicate that the smoker is leaving
    printf("\n");
    printf("Smoker with tobacco is hungry and is leaving...\n");
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
        while (sem_wait(&paperSem) != 0) {} // 'P' (wait) for paperSem semaphore

        {
            // make cigarette
            printf("Smoker with paper is making a cigarette..\n");

            // Generate random number up to 50 milliseconds
            sleepTime = rand()% 50000;
            usleep(sleepTime);
        }

        sem_post(&agentSem); // 'V' agentSem

        {
            // smoke the cigarette
            printf("Smoker with paper is smoking a cigarette..\n");

            // Generate random number up to 50 milliseconds
            sleepTime = rand()% 50000;
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
        while (sem_wait(&matchSem) != 0) {} // 'P' (wait) for matchSem semaphore

        {
            // make cigarette
            printf("Smoker with matches is making a cigarette..\n");

            // Generate random number up to 50 milliseconds
            sleepTime = rand()% 50000;
            usleep(sleepTime);
        }

        sem_post(&agentSem); // 'V' agentSem

        {
            // smoke the cigarette
            printf("Smoker with matches is smoking a cigarette..\n");

            // Generate random number up to 50 milliseconds
            sleepTime = rand()% 50000;
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