#include "StatisticsThread.h"
#include <iostream>
#include <future>
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>

StatisticsThread::StatisticsThread(string n) {
	name = n;
}

void StatisticsThread::run(BoundedBuffer& bb) {

	std::unordered_map<string, int> counters;
	cout << "STAT THREAD STARTED " << endl;
	
	while(true) {
		std::chrono::system_clock::time_point one = std::chrono::system_clock::now() + std::chrono::seconds(1);

		auto promise = std::promise<string>();
		auto f = promise.get_future();

		thread([&]
		{
			promise.set_value(bb.pop());
			cout << "POPPED!" << endl;
		}).detach();

		if(std::future_status::ready == f.wait_until(one)) { 
			//std::cout << "Completed!" << endl;
		}
		else
		{
			promise.set_value("");
			f.get();
			cout << "STAT EXITING" << endl;
			break;
		}

		string data = f.get();
		counters[data]++;
	}
	cout << "EXITED STAT THREAD" << endl;
	string filename = name + ".txt";
	ofstream ost{filename, ofstream::out};

	vector<string> key_list;
	for (auto it=counters.begin(); it != counters.end(); ++it) {
    	key_list.push_back(it->first);
	}
	sort(key_list.begin(), key_list.end());

	for(auto kv : key_list) {
		ost << name << ": " << kv << " appeared " << counters[kv] << " time(s)" << endl;
	}
	ost.close();
}
