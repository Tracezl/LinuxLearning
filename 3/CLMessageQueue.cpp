#include "CLMessageQueue.h"
#include "CLCriticalSection.h"
#include "CLMessage.h"
#include "CLLog.h"

CLMessageQueue::CLMessageQueue() : m_Event(true)
{
}

CLMessageQueue::~CLMessageQueue()//析构函数清空消息队列
{
	while(true)
	{
		CLMessage *pMsg = Pop();  //如果队列有消息，将消息出队
		if(pMsg == 0)  //消息为空直接退出
			break;
        delete pMsg;   //删除消息
	}
}

CLStatus CLMessageQueue::PutMessage(CLMessage * pMessage)
{
	CLStatus s = Push(pMessage);
	if(!s.IsSuccess())
	{
		CLLog::WriteLogMsg("In CLMessageQueue::PutMessage(), Push error", 0);
		return CLStatus(-1, 0);
	}

	CLStatus s2 = m_Event.Set();
	if(!s2.IsSuccess())
	{
		CLLog::WriteLogMsg("In CLMessageQueue::PutMessage(), m_Event.Set error", 0);
		return CLStatus(-1, 0);
	}
	return CLStatus(0, 0);
}

CLMessage* CLMessageQueue::GetMessage()
{
	CLStatus s = m_Event.Wait();//唤醒线程
	if(!s.IsSuccess())
	{
		CLLog::WriteLogMsg("In CLMessageQueue::GetMessage(), m_Event.Wait error", 0);
		return 0;
	}
    return Pop();
}
/**
要保证读写的安全性，因此需要使用互斥量
*/
CLStatus CLMessageQueue::Push(CLMessage * pMessage)
{
	try
	{
		CLCriticalSection cs(&m_Mutex);//写互斥量，保证写安全
        my_MessageQueue.Push(pMessage);//消息入队
	}
	catch(const char* str)
	{
		CLLog::WriteLogMsg("In CLMessageQueue::Push(), exception arise", 0);
		delete pMessage;
		return CLStatus(-1, 0);
	}
    return CLStatus(0, 0);
}

CLMessage* CLMessageQueue::Pop()
{
	try
	{
		CLCriticalSection cs(&m_Mutex);//读互斥量，保证读安全
        if(my_MessageQueue.IsEmpty())//从空队列读会异常，需要先判断队列是否为空
			return 0;
        CLMessage *p = my_MessageQueue.Pop();
		return p;
	}
	catch(const char* str)
	{
		CLLog::WriteLogMsg("In CLMessageQueue::Pop(), exception arise", 0);
		return 0;
	}
}
