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
#include "BoundedBuffer.h"
#include "semaphore.h"

using namespace std;

BoundedBuffer::BoundedBuffer(int maxSize) {
	this.maxSize = maxSize;
	mutex(1);
	fullSlots(0);
	emptySlots(maxSize);
}
void BoundedBuffer::push(string s) {
	emptySlots.P();
	mutex.P();
	buffer.push_back(s);
	mutex.V();
	fullSlots.V();
}
string BoundedBuffer::pop() {
	fullSlots.P();
	mutex.P();
	string val = buffer.back();
	buffer.pop_back();
	mutex.V();
	emptySlots.V();
	return val;
}