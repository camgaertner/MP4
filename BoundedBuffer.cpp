/* 
    BoundedBuffer.cpp
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

BoundedBuffer::BoundedBuffer(int maxSize) {
	this.maxSize = maxSize;
	mutex(1);
	fullSlots(0);
	emptySlots(maxSize);
}
void BoundedBuffer::push(string s) {
	empty.P();
	mutex.P();
	buffer.push_back(s);
	mutex.V();
	full.V();
}
string BoundedBuffer::pop() {
	full.P();
	mutex.P();
	// 
	mutex.V();
	empty.V();
}