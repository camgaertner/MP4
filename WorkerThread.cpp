#include "WorkerThread.h"
#include <iostream>
#include <future>

using namespace std;

void WorkerThread::run(BoundedBuffer& bb, RequestChannel& rc, vector<BoundedBuffer>& responseBuffers) {

	string name = rc.send_request("newthread");
	cout << "~~~~~~~~~~~~~~NAME IS~~~~~~~~~ " << name << endl;
	RequestChannel mychan (name, RequestChannel::CLIENT_SIDE);
	
	while(true) {
	
		std::chrono::system_clock::time_point one = std::chrono::system_clock::now() + std::chrono::seconds(1);

		auto promise = std::promise<string>();
		auto f = promise.get_future();

		thread([&]
		{
			promise.set_value(bb.pop());
		}).detach();

		/*std::future<string> fut = std::async (std::launch::async, [&](){ return bb.pop(); });
		
		string answer = "";
		thread t ([&](answer&) { answer = bb.pop(); }).detach();
	*/	
		if(std::future_status::ready == f.wait_until(one)) { 
			std::cout << "Completed!" << endl;
		}
		else
		{
			cout << "I'm going to quit" << endl;
			mychan.send_request("quit");
			promise.set_value("");
			break;
		}
		string data = f.get();

		string reply = mychan.send_request("data " + data);
		
		cout << "Reply is " << reply << endl;

		if(data == "Joe Smith") {
			responseBuffers[0].push(reply);
		}
		else if(data == "Jane Smith") {
			responseBuffers[1].push(reply);
		} 
		else if(data == "John Doe"){
			responseBuffers[2].push(reply);
		}
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
