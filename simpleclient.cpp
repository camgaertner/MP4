/* 
    File: simpleclient.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2013/01/31

    Simple client main program for MP3 in CSCE 313
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
#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#include <unistd.h>

#include "reqchannel.h"
#include "RequestThread.h"
#include "WorkerThread.h"
#include "StatisticsThread.h"
#include "BoundedBuffer.h"

#include <thread>
#include <future>         // std::async, std::future
#include <chrono> 
#include <functional>
#include <vector>

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
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

void pause_thread(int n) 
{
	sleep(1);
}

int main(int argc, char * argv[]) {

	int pid = fork();
	if(pid == 0) {
		execvp("./dataserver", NULL);
		return 0;
	}
  // fill ints from arguments later
	int data_requests = 10;
	int bounded_buffer_size = 5;
	int worker_threads = 4;
	cout << "CLIENT STARTED:" << endl;

	cout << "Establishing control channel... " << flush;
	RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
	cout << "done." << endl;
  
	BoundedBuffer requests(bounded_buffer_size);
	
	BoundedBuffer response1(bounded_buffer_size);
	BoundedBuffer response2(bounded_buffer_size);
	BoundedBuffer response3(bounded_buffer_size);
	
	vector<BoundedBuffer> buffers = {response1, response2, response3};
	
	RequestThread r1 ("Joe Smith", data_requests);
	RequestThread r2 ("Jane Smith", data_requests);
	RequestThread r3 ("John Doe", data_requests);

	StatisticsThread s1 ("Joe Smith");
	StatisticsThread s2 ("Jane Smith");
	StatisticsThread s3 ("John Doe");
	
	vector<thread> threads;
	
	threads.push_back(thread([&]() { r1.run(requests); }));
	threads.push_back(thread([&]() { r2.run(requests); }));
	threads.push_back(thread([&]() { r3.run(requests); }));
	

	for(int i = 0; i < worker_threads; i++) {
		cout << "CREATING THREAD" << endl;
		string str = chan.send_request("newthread");
		threads.push_back(thread([&]() { 
			WorkerThread().run(requests, str, buffers);
		}));
		cout << "CREATED THREAD ~~~~!!" << endl;
	}

	threads.push_back(thread([&]() { s1.run(buffers[0]); }));
	threads.push_back(thread([&]() { s2.run(buffers[1]); }));
	threads.push_back(thread([&]() { s3.run(buffers[2]); }));
	
	cout << "Waiting to join threads" << endl;
	
	for(int i = 0; i < threads.size(); i++) {
		threads[i].join();
		cout << "Joined one thread" << endl;
	}
	
	cout << "I'm done" << endl;
	
	string reply4 = chan.send_request("quit");
	usleep(1000000);

  /* -- Start sending a sequence of requests */

  /*string reply1 = chan.send_request("hello");
  cout << "Reply to request 'hello' is '" << reply1 << "'" << endl;

  string reply2 = chan.send_request("data Joe Smith");
  cout << "Reply to request 'data Joe Smith' is '" << reply2 << "'" << endl;

  string reply3 = chan.send_request("data Jane Smith");
  cout << "Reply to request 'data Jane Smith' is '" << reply3 << "'" << endl;

  string reply5 = chan.send_request("newthread");
  cout << "Reply to request 'newthread' is " << reply5 << "'" << endl;
  RequestChannel chan2(reply5, RequestChannel::CLIENT_SIDE);

  string reply6 = chan2.send_request("data John Doe");
  cout << "Reply to request 'data John Doe' is '" << reply6 << "'" << endl;

  string reply7 = chan2.send_request("quit");
  cout << "Reply to request 'quit' is '" << reply7 << "'" << endl;
  
  cout << "Reply to request 'quit' is '" << reply4 << "'" << endl;

  usleep(1000000);*/
}
