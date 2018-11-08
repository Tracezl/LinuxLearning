#include "CLExecutiveFunctionProvider.h"
#include "CLExecutiveFunctionForMsgLoop.h"
#include "CLExecutiveNameServer.h"
#include "CLThread.h"
#include "CLThreadProxy.h"
#include "CLMessageQueue.h"
#include "CLMessage.h"
#include "CLMsgLoopManagerForMsgQueue.h"
#include "CLMessageObserver.h"
#include <iostream>

using namespace std;

#define SUB_MSG 0  //减法消息标识符
#define QUIT_MSG 1 //退出消息标识符

class CLMsgJob;

class CLSUBMessage : public CLMessage//继承CLMessage的减法消息
{
private:
	int Op1;
	int Op2;
	
public:
    	friend class CLMsgJob;
    CLSUBMessage(int Op1, int Op2):CLMessage(SUB_MSG)//构造函数初始化
	{
		this->Op1 = Op1;
		this->Op2 = Op2;
	}

	virtual ~CLSUBMessage()
	{
	}
};

class CLQuitMessage : public CLMessage  //继承CLMessage的退出消息
{
public:
    CLQuitMessage() : CLMessage(QUIT_MSG)
    {
	}

	virtual ~CLQuitMessage()
	{
	}
};

class CLMsgJob : public CLMessageObserver//CLMsgJob继承CLMessageObserver，是线程执行体
{
public:
	CLMsgJob()
	{
	}

	virtual ~CLMsgJob()
	{
	}

	virtual CLStatus Initialize(CLMessageLoopManager *pMessageLoop, void* pContext)
	{
		//注册回调函数到线程，用于绑定，可以找到具体做什么操作
		pMessageLoop->Register(SUB_MSG, (CallBackForMessageLoop)(&CLMsgJob::Do_SubMsg));
		pMessageLoop->Register(QUIT_MSG, (CallBackForMessageLoop)(&CLMsgJob::Do_QuitMsg));

		return CLStatus(0, 0);
	}

	CLStatus Do_SubMsg(CLMessage *pM)
	{
		CLSUBMessage *pSubMsg = (CLSUBMessage *)pM;
		
		cout << pSubMsg->Op1 << "-" << pSubMsg->Op2 << "=" << pSubMsg->Op1 - pSubMsg->Op2 << endl; //执行减法操作

		return CLStatus(0, 0);
	}	

	CLStatus Do_QuitMsg(CLMessage *pM)
	{
		cout << "quit..." << endl;//退出消息
		
		return CLStatus(QUIT_MESSAGE_LOOP, 0);
	}
};

int main()
{	
	CLThreadProxy proxy(new CLMsgJob, "son_Thread", true);//库，创建线程
	
	proxy.Run(0);

	CLExecutiveNameServer::PostExecutiveMessage("son_Thread", new CLSUBMessage(7, 1));//向son_Thread线程发送消息
	CLExecutiveNameServer::PostExecutiveMessage("son_Thread", new CLSUBMessage(9, 2));
	CLExecutiveNameServer::PostExecutiveMessage("son_Thread", new CLSUBMessage(2, 6));
	CLExecutiveNameServer::PostExecutiveMessage("son_Thread", new CLQuitMessage());

	return 0;
}
