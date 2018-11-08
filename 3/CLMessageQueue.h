#ifndef CLMessageQueue_H
#define CLMessageQueue_H

#include "CLMyQueue.h"
#include "CLStatus.h"
#include "CLMutex.h"
#include "CLEvent.h"

class CLMessage;

/*
该类是中的消息队列是自定义实现的CLMyQueue
*/
class CLMessageQueue
{
private:
	CLMyQueue<CLMessage*> my_MessageQueue;//使用自定义的CLMyQueue队列，队列中的数据类型为CLMessage*
	CLMutex m_Mutex;
	CLEvent m_Event;
	
public:
	CLMessageQueue();
	virtual ~CLMessageQueue();

public:
	CLStatus PutMessage(CLMessage * pMessage);  //消息入队接口
	CLMessage* GetMessage(); //消息出队接口

private:
	CLStatus Push(CLMessage * pMessage);//消息写入队列
	CLMessage* Pop();//从队列中读出消息
};

#endif