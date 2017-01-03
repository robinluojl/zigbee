#ifndef _ZIGBEEADAPTER_H
#define _ZIGBEEADAPTER_H

#include "zigbee/Serial_HardDriver.h"
#include "zigbee/queue.h"
#include <stdlib.h>
#include <ros/ros.h>
#include <pthread.h>
#include <iostream>

typedef struct SDKFilter
{
  int recvIndex;
  unsigned char recvBuf[1024];
}SDKFilter;

using namespace std;

class ZigbeeAdapter
{
private:
	pthread_t m_Tid;
	void byteHandler(const uint8_t in_data);

public:
	Queue *send_queue;												//for send
	SDKFilter filter;													//for receive
	Serial_HardDriver *m_hd;

	ZigbeeAdapter();

	~ZigbeeAdapter();

	void init(string device, unsigned int baudrate);

	void readPoll();
	void sendPoll();
};

static void* SerialThread(void* param)
{
	ZigbeeAdapter* p_adapter = (ZigbeeAdapter*)(param);

	while(true)
	{
		p_adapter->readPoll();
		p_adapter->sendPoll();
		//cout<<'!'<<endl;
		usleep(100000);
	}
}

extern ZigbeeAdapter *adapter;

#endif
