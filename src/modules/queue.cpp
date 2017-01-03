#include "zigbee/queue.h"
#include "string.h"

Queue::Queue(unsigned int size)
{
	this->size = size;
	Data = new unsigned char[size];
}

Queue::~Queue(void)
{
	delete []Data;
	Data = NULL;
}

/*
*********************************************************************************************************
* Function    : SetEmpty
* 
* Description : 清空队列
*
* Arguments   :
*
* Returns     : 
*********************************************************************************************************
*/
void Queue::SetEmpty()
{
    this->front = 0;        
    this->rear = 0;
}
/*
*********************************************************************************************************
* Function    : EnQueue
* 
* Description : 入队函数
*
* Arguments   :
*
* Returns     : 
*********************************************************************************************************
*/
void Queue::EnQueue(unsigned char Data)
{
    if (IsFull())
    {
        return;
    }
	this->Data[rear] = Data;
    rear = (rear + 1) % size;
}
/*
*********************************************************************************************************
* Function    : DeQueue
*
* Description : 出队函数
*
* Arguments   :
*
* Returns     : 
*********************************************************************************************************
*/
void Queue::DeQueue(unsigned char * data)
{
    if (IsEmpty())
    {
        return;
    }
		*data = Data[front];
//    memcpy(Data, &queue->Data[queue->front], sizeof(unsigned char));
    front = (front + 1) % size;
}
/*
*********************************************************************************************************
* Function    : GetQueueLength
*
* Description : 获取队列长度
*
* Arguments   :
*
* Returns     : 
*********************************************************************************************************
*/
int Queue::GetQueueLength()
{
    if(rear > front) 
        return (rear - front);
    else if (rear < front) 
        return (size - (front - rear) + 1);
    else
        return 0;

} 

/*
*********************************************************************************************************
* Function    : RealignQueue
*
* Description : 重新排列队列
*
* Arguments   :
*
* Returns     : 
*********************************************************************************************************
*/
int Queue::RealignQueue()
{
	unsigned char *temp = new unsigned char[size];
	int i,temp1;
	if(front>rear)
	{
		memcpy(temp,Data,(rear)*sizeof(unsigned char));
		temp1 = size - front+1;
		for(i=0;i<temp1;i++)
			Data[i] = Data[i+front];
		memcpy(Data + temp1,temp,(rear)*sizeof(unsigned char));
		rear = temp1 + rear;
		front = 0;
	}
	delete temp;
	temp = 0;
	return 0;

} 
