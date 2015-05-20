#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <signal.h>
#include "donuts.hpp"

using namespace std;
int shmid, semid[3];

/*
 * 
 */

void sig_handler(int sig) {
    printf("In signal handler with signal # %d\n", sig);

    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        perror("handler failed shm RMID: ");
    }
    for (int i = 0; i < NUMSEMIDS; i++) {
        if (semctl(semid[i], 0,
                IPC_RMID, (union semun *)0) == -1) {
            perror("handler failed sem RMID: ");
        }
    }
    printf("Clean Up done. Exiting..");
    exit(5);
}

int main(int argc, char** argv) {

    sigset_t mask_sigs;
    struct sigaction new_;
    int i, nsigs, rand;
    struct donut_ring *shared_ring;

    int sigs [] = {SIGHUP, SIGINT, SIGQUIT, SIGBUS,
        SIGTERM, SIGSEGV, SIGFPE};

    int in_ptr [NUMFLAVORS];
    int serial [NUMFLAVORS];
    struct timeval randtime;

    /* producer initializes serial counters and in-pointers  */
    for (i = 0; i < NUMFLAVORS; i++) {
        in_ptr [i] = 0;
        serial [i] = 0;
    }

    nsigs = sizeof (sigs) / sizeof (int);
    sigemptyset(&mask_sigs);

    for (i = 0; i < nsigs; i++)
        sigaddset(&mask_sigs, sigs [i]);

    for (i = 0; i < nsigs; i++) {
        new_.sa_handler = sig_handler;
        new_.sa_mask = mask_sigs;
        new_.sa_flags = 0;
        if (sigaction(sigs [i], &new_, NULL) == -1) {
            perror("can't set signals: ");
            exit(1);
        }
    }

    if ((shmid = shmget(MEMKEY, sizeof (struct donut_ring),
            IPC_CREAT | 0600)) == -1) {
        perror("shared get failed: ");
        exit(1);
    }

    try {
        if ((shared_ring = (struct donut_ring *) shmat(shmid, NULL, 0)) == (struct donut_ring *) - 1) { //What???
            throw Exception("shared attach failed: ");
        }

        for (i = 0; i < NUMSEMIDS; i++) {
            if ((semid[i] = semget(SEMKEY + i, NUMFLAVORS,
                    IPC_CREAT | 0600)) == -1) {
                throw Exception("semaphore allocation failed: ");
            }

        }
        gettimeofday(&randtime, (struct timezone *) 0);
        /* use microsecond component for uniqueness */

        unsigned short xsub1[3];
        xsub1[0] = (ushort) randtime.tv_usec;
        xsub1[1] = (ushort) (randtime.tv_usec >> 16);
        xsub1[2] = (ushort) (getpid());

        /* use nrand48 with xsub1 to get 32 bit random number */

        /* use the semsetall utility to set initial semaphore values */

        if (semsetall(semid [PROD], NUMFLAVORS, NUMSLOTS) == -1) {
            throw Exception("semsetsall failed: ");
        }
        if (semsetall(semid [CONSUMER], NUMFLAVORS, 0) == -1) {
            throw Exception("semsetsall failed: ");
        }
        if (semsetall(semid [OUTPTR], NUMFLAVORS, 1) == -1) {
            throw Exception("semsetsall failed: ");
        }

        cout << "Producing Donuts.." << endl;
        
        while (true) {

            rand = nrand48(xsub1) & 3;

            if (p(semid[PROD], rand) == -1)sig_handler(-1); //lock prod

            //producing
            serial[rand] = serial[rand] + 1;
            shared_ring->flavor[rand][in_ptr[rand]] = serial[rand];
            in_ptr[rand] = (in_ptr[rand] + 1) % NUMSLOTS;

            if (v(semid[CONSUMER], rand) == -1) sig_handler(-1); //done
        }

    } catch (Exception e) {
        cout << e.what() << endl;
        sig_handler(-1);
    }
    return 0;
}
