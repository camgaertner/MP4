#include "StatisticsThread.h"
#include <iostream>
#include <future>
#include <unordered_map>
#include <string>

StatisticsThread::StatisticsThread(string n) {
	name = n;
}

void StatisticsThread::run(BoundedBuffer& bb) {

	std::unordered_map<string, int> counters;
	
	while(true) {
		std::chrono::system_clock::time_point one = std::chrono::system_clock::now() + std::chrono::seconds(1);

		auto promise = std::promise<string>();
		auto f = promise.get_future();

		thread([&]
		{
			promise.set_value(bb.pop());
		}).detach();

		if(std::future_status::ready == f.wait_until(one)) { 
			std::cout << "Completed!" << endl;
		}
		else
		{
			cout << "STAT going to quit" << endl;
			promise.set_value("");
			break;
		}

		string data = f.get();
		cout << "Stat thread got " << data << " as data ?????" << endl;
		counters[data]++;
	}
	
	for(auto kv : counters) {
		cout << name << ": " << kv.first << " appeared " << kv.second << " time(s)" << endl;
	}
	
	cout << "stat thread is done ~~~~~~~~~~====" << endl;
}
