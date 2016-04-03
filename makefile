# makefile

all: dataserver simpleclient

reqchannel.o: reqchannel.h reqchannel.cpp
	g++ -c -g reqchannel.cpp

dataserver: dataserver.cpp reqchannel.o 
	g++ -g -o dataserver dataserver.cpp reqchannel.o -lpthread

simpleclient: simpleclient.cpp RequestThread.cpp BoundedBuffer.cpp Semaphore.cpp WorkerThread.cpp StatisticsThread.cpp reqchannel.o
	g++ -std=c++11 -pthread -g -o simpleclient simpleclient.cpp RequestThread.cpp BoundedBuffer.cpp Semaphore.cpp WorkerThread.cpp StatisticsThread.cpp reqchannel.o
