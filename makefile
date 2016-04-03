# makefile

all: dataserver simpleclient

reqchannel.o: reqchannel.h reqchannel.cpp
	g++-4.7 -c -g reqchannel.cpp

dataserver: dataserver.cpp reqchannel.o 
	g++-4.7 -g -o dataserver dataserver.cpp reqchannel.o -lpthread

simpleclient: simpleclient.cpp RequestThread.cpp BoundedBuffer.cpp Semaphore.cpp WorkerThread.cpp reqchannel.o
	g++-4.7 -std=c++11 -pthread -g -o simpleclient simpleclient.cpp RequestThread.cpp BoundedBuffer.cpp Semaphore.cpp WorkerThread.cpp reqchannel.o
