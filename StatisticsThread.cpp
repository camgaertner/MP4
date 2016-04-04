#include "StatisticsThread.h"
#include <iostream>
#include <future>
#include <unordered_map>
#include <string>
#include <fstream>

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
			//std::cout << "Completed!" << endl;
		}
		else
		{
			promise.set_value("");
			break;
		}

		string data = f.get();
		counters[data]++;
	}
	string filename = name + ".txt";
	ofstream ost{filename, ofstream::out};
	cout << "aaaaa" << endl;
	for(auto kv : counters) {
		ost << name << ": " << kv.first << " appeared " << kv.second << " time(s)" << endl;
	}
}
