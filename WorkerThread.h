#include "BoundedBuffer.h"
#include "reqchannel.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#include <unistd.h>
#include <string>
#include <vector>

class WorkerThread {
	
public:
	
	void run(BoundedBuffer&, string, vector<BoundedBuffer>&);
	
private:
	string name;
	int numRequests;

};
