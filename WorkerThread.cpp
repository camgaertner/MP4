#include "WorkerThread.h"
#include <iostream>
#include <future>

using namespace std;

void WorkerThread::run(BoundedBuffer& bb, RequestChannel& rc, vector<BoundedBuffer>& responseBuffers) {
	
	RequestChannel mychan (rc.send_request("newthread"), RequestChannel::CLIENT_SIDE);
	bool keepGoing = true;
	
	
	while(true) {
	
		//std::chrono::system_clock::time_point one = std::chrono::system_clock::now() + std::chrono::seconds(1);

		std::future<string> fut = std::async (std::launch::async, [&](){ return bb.pop(); });
		
		string answer = "";
		thread t ([&](answer&) { answer = bb.pop(); }).detach();
		
		/*if(std::future_status::ready == fut.wait_until(one)) { 
			std::cout << "Completed!" << endl;
			break;
		}*/
		//else {
			cout << "I'm going to quit" << endl;
			mychan.send_request("quit");
			cout << "About to quit" << endl;
			keepGoing = false;
			cout << "About to quit" << endl;
			exit(0);
			break;
		//}
	}
	
	
	//mychan.send_request("quit");
	//usleep(1000000);
	
	//get off the buffer
	/*while(keepGoing) {
		std::chrono::system_clock::time_point one = std::chrono::system_clock::now() + std::chrono::seconds(1);

		std::future<string> fut = std::async (std::launch::async, [&](){ return bb.pop(); });
		
		if(std::future_status::ready == fut.wait_until(one)) { 
			std::cout << "Completed!" << endl; 
		}
		else {
			cout << "I'm going to quit" << endl;
			mychan.send_request("quit");
			usleep(1000000);
			cout << "About to quit" << endl;
			keepGoing = false;
			cout << "About to quit" << endl;
		}
		
		string data = fut.get();
		cout << "After future" << endl;
		
		string reply = mychan.send_request("data " + data);
		
		cout << "Reply for " << data <<  " is " << reply << endl;
		
		//put into response buffer;
		/*if(data == "Joe Smith") {
			responseBuffers[0].push(reply):
		}
		else if(data == "Jane Smith") {
			responseBuffers[1].push(reply):
		} 
		else if(data == "John Doe"){
			responseBuffers[2].push(reply):
		}*/
	//}
	
	cout << "I'm outside the loop" << endl;
}