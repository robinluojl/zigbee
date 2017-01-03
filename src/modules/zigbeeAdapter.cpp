#include "zigbee/zigbeeAdapter.h"
#include "zigbee/ZigbeeNode.h"

#include <string.h>

ZigbeeAdapter::ZigbeeAdapter()
{

}

ZigbeeAdapter::~ZigbeeAdapter()
{
	//按逆向注销
	//void *retval;
	//pthread_join(m_Tid,&retval);

	delete []send_queue;
	delete m_hd;
}

void ZigbeeAdapter::init(string device, unsigned int baudrate)
{
	cout<<"---Zigbee connection Info---" << endl;
	cout<<"Serial port: "<< device.c_str() << endl;
	cout<<"baudrate: " << baudrate << endl;
	cout<<"--------"<<endl;

	m_hd = new Serial_HardDriver(device, baudrate);
  m_hd->init();

//发送与接收缓冲清空
	send_queue = new Queue(500);
	send_queue->SetEmpty();
	filter.recvIndex = 0;

	int ret;
	ret = pthread_create(&m_Tid, 0,SerialThread,(void*)this);		//zigbee收发线程
	if(0 != ret)
		ROS_FATAL("Cannot create new thread for zigbee serial!");
	else
		ROS_INFO("Succeed to create thread zigbee serial");
}

void ZigbeeAdapter::sendPoll()
{
	size_t a ;

/*	for(int i=0;i<SendMsgNum;i++)
	{
		//此处应禁止修改SendMsgQueue
		if(SendMsgQueue[i].UpdateFlag > 0)		//数据更新了
		{
			m_hd->lockMemory();
			a = SendMsgQueue[i].msg.length();				//get length
			m_hd->send(SendMsgQueue[i].msg,a);												//write
			m_hd->freeMemory();
			SendMsgQueue[i].UpdateFlag = 0;
		}
		//允许修改SendMsgQueue
	}
	*/
	unsigned char buf[500];

	m_hd->lockMemory();
	//给队列上锁
	a = send_queue->GetQueueLength();
	for(int i=0;i<a;i++)
		send_queue->DeQueue(buf+i);
	//队列解锁

	m_hd->send(buf,a);
	m_hd->freeMemory();
}


#define BUFFER_SIZE 1024

void ZigbeeAdapter::readPoll()
{
	int read_len;
	uint8_t buf[BUFFER_SIZE];
	read_len = m_hd->readall(buf,BUFFER_SIZE);

	for (int i = 0; i < read_len; i++)
  {
		cout<<hex<<(uint16_t)buf[i]<<endl;
    byteHandler(buf[i]);			//对每一位数据处理函数
  }
}


//该函数与数据格式有关
void ZigbeeAdapter::byteHandler(const uint8_t in_data)
{
//	cout<<in_data<<endl;

	uint16_t temp1;
	uint8_t temp2;
	bool temp_state;

	if(filter.recvIndex == 0)
	{
		if(in_data == 0xFD)
		{
			//cout<<"receive 0xFE"<<endl;
			filter.recvBuf[filter.recvIndex] = in_data;
			filter.recvIndex++;
		}
	}
	else		//已经顺利收到包头
	{
		filter.recvBuf[filter.recvIndex] = in_data;
		filter.recvIndex++;
		if(filter.recvIndex == 4)		//看看是不是有效的ID
		{
			temp1 = (((uint16_t)filter.recvBuf[3])<<8)|filter.recvBuf[2];			//zigbee_ID
			temp_state = search_uavIDbyZigbeeID(temp1,temp2);

			if(temp_state==false)
				filter.recvIndex = 0;		//error 清空
		}

		if(filter.recvIndex >= 1)														//已收到数据包长度位
		{
			if(filter.recvIndex == (filter.recvBuf[1]+6))     //长度收满,长度校验完成
			{
				zigbeeNode->HandleData(&filter);								//数据处理 校验 过滤 解码 消息发布等
				filter.recvIndex = 0;		//清空
			}
		}
	}
}
