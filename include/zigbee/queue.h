#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdlib.h>

#define IsEmpty() (this->front == this->rear)
#define IsFull()  (((this->rear + 1) % this->size) == this->front)

class Queue
{
private:
	int     front;
  int     rear;
  unsigned char *Data; 

	unsigned int size;
public:
	Queue(unsigned int size);
	~Queue(void);

	void SetEmpty();
	void EnQueue(unsigned char Data);
	void DeQueue(unsigned char * Data);
	int GetQueueLength();
	int RealignQueue();
};


#endif
