#ifndef CLEVENT_H
#define CLEVENT_H

#include "CLStatus.h"
#include "CLMutex.h"
#include "CLConditionVariable.h"

/*
默认情况下， 创建一个初始无信号，自动重置信号的信号（用于唤醒一个 等待线程）

在构造函数中，若bSemaphore被设置为true，则调用几次Set，即可唤醒几次或个线程
*/
class CLEvent
{
public: 
	/*
	构造函数和析构函数出错时，会抛出字符串类型异常
	*/
	CLEvent( );
	explicit CLEvent(bool bSemaphore);
	virtual ~CLEvent();

public: 
	CLStatus Set();

	CLStatus Wait();

private:
	CLEvent(const CLEvent&);
	CLEvent& operator=(const CLEvent&);

private:
	CLMutex m_Mutex;
	CLConditionVariable m_Cond;
	volatile int m_Flag;

	bool m_bSemaphore;
};

#endif
