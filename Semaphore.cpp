/* 
    semaphore.cpp
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <iostream>
#include "semaphore.h"

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS */
/*--------------------------------------------------------------------------*/

Semaphore::Semaphore(int _val) {
	value = _val;
	pthread_mutex_init(&m);
	pthread_cond_init(&c);
}

int Semaphore::P() {
	Lock();
	while(value <= 0) {
		pthread_cond_wait(&c, &m);
	}
	--value;
	Unlock();
}

int Semaphore::V() {
	Lock();
	int old_value = value;
	++value;
	Unlock();
	
	if(old_value ==0) {
		pthread_cond_signal(&c);
	}
}