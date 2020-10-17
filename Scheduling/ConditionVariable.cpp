#include "mbed.h"
#include <cstdint>
#include <cstdio>

//Condition Variable - provides a mechanism to wait/signal state changes. 

//Protect shared resources with mutex, release mutex with change in data. 
//else this can lead to a race condition. 

//Handle the wait for a state change. Cannot wait on multiple event. Release and 
//acquire the mutex.

//notify_all unblocks all of these threads. 

//helps to prevent race conditions.

Mutex mutex;
ConditionVariable cond(mutex);

uint32_t counter = 0;
bool done = false;

void worker_thread(){
    mutex.lock();
    do{
        printf("Worker: Count %u\r\n", counter);
    } while(!done);
    printf("Worker: Exiting\r\n");
    mutex.unlock();
}

int main(){
    Thread thread;
    thread.start(worker_thread);
    for(int i = 0; i < 5; i++){
        mutex.lock();
        counter++;
        printf("Main: set count to %u\r\n", counter);
        cond.notify_all();
        mutex.unlock();
        ThisThread::sleep_for(1s);
    }
    mutex.lock();
    done = true;
    printf("Main: set done \r\n");
    cond.notify_all();
    mutex.unlock();
    thread.join();
}