/**
 * Talik Kasozi     talik.aziizi@gmailcom
 * credit ~/bill/308/call_help_assign3.txt
 * 
 * Consumer.cpp
 * 
 */
#include <time.h>
#include <stdlib.h>
#include <unistd.h>  
#include <cstdlib>
#include <signal.h>
#include <iostream>
#include <string>

#include "donuts.hpp"

using namespace std;

void consumer(struct donut_ring *, int[]);

void consumer(struct donut_ring *shared, int sem_id[]) {
    int inner_rep[NUMFLAVORS][12];

    struct timeval randtime;
    unsigned short xsub1[3];
    xsub1[0] = (ushort) randtime.tv_usec;
    xsub1[1] = (ushort) (randtime.tv_usec >> 16);
    xsub1[2] = (ushort) (getpid());

    char buf[256];
    struct tm *loctime;
    time_t rawtime;
    rawtime = time(NULL);
    loctime = localtime(&rawtime);
    strftime(buf, 256, "%I:%M %p.", loctime);

    int rand, jelly, plain, coco, honey;
    pid_t pid = getpid();

    for (int y = 0; y < NUMFLAVORS; y++) {
        for (int j = 0; j < 12; j++) {
            inner_rep[y][j] = -1;
        }
    }

    for (int dozen_num = 1; dozen_num <= 10; dozen_num++) {

        jelly = 0, plain = 0, coco = 0, honey = 0;

        for (int i = 0; i < 12; i++) {

            rand = nrand48(xsub1) & 3;

            if (p(sem_id[OUTPTR], rand) == -1) exit(2);
            if (p(sem_id[CONSUMER], rand) == -1) exit(2); //lock semaphore

            //Consuming
            int donut = shared->flavor[rand][shared->outptr[rand]];

            switch (rand) {
                case 0: //plain
                    inner_rep[rand][jelly] = donut; //use buffer
                    jelly++;
                    break;
                case 1://jelly
                    inner_rep[rand][plain] = donut; //use buffer
                    plain++;
                    break;
                case 2://coconut
                    inner_rep[rand][coco] = donut; //use buffer
                    coco++;
                    break;
                case 3://honey-dip
                    inner_rep[rand][honey] = donut; //use buffer
                    honey++;
                    break;
                default:

                    break;
            }

            shared->outptr[rand] = (shared->outptr[rand] + 1) % NUMSLOTS;

            if (v(sem_id[PROD], rand) == -1) exit(2); //release            
            if (v(sem_id[OUTPTR], rand) == -1) exit(2);

        } //End of 12 dozen loop

        cout << "Process Pid: " << pid << " time: " << buf << " dozen #: " << dozen_num << "\n" << endl;
        cout << "plain\t" << "jelly\t" << "coconut\t" << "honey-dip" << endl;


        for (int x = 0; x < 12; x++) {
            (inner_rep[0][x] > -1) ? cout << " " << inner_rep[0][x] << "\t" : cout << "\t";
            (inner_rep[1][x] > -1) ? cout << "  " << inner_rep[1][x] << "\t" : cout << "\t";
            (inner_rep[2][x] > -1) ? cout << " " << inner_rep[2][x] << "\t" : cout << "\t";
            (inner_rep[3][x] > -1) ? cout << " " << inner_rep[3][x] << "\t" << endl : cout << endl;
        }//display

        for (int y = 0; y < NUMFLAVORS; y++) {
            for (int j = 0; j < 12; j++) {
                inner_rep[y][j] = -1;
            }
        }//set inner_rep to 0
        
        cout << endl;

    }//end of 10 dozens
}

/*
 * 
 */
int main(int argc, char** argv) {
    int shmid, semid[3];
    struct donut_ring *shared_ring;

    try {

        if ((shmid = shmget(MEMKEY, sizeof (struct donut_ring),
                IPC_CREAT | 0600)) == -1) {
            throw Exception("shared get failed: ");
        }

        if ((shared_ring = ((struct donut_ring *) shmat(shmid, NULL, 0))) == (struct donut_ring *) - 1) { //What???
            throw Exception("shared attach failed: ");
        }

        for (int i = 0; i < NUMSEMIDS; i++)
            if ((semid[i] = semget(SEMKEY + i, NUMFLAVORS, IPC_CREAT | 0600)) == -1)
                throw Exception("->Semaphore Allocation Failed");

    } catch (Exception e) {

        cout << e.what() << endl;
        exit(1);
    }

    consumer(shared_ring, semid);

    return 0;
}