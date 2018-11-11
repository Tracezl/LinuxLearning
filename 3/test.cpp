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

#define MUL_MSG 0  //乘法消息标识符
#define QUIT_MSG 1 //退出消息标识符

class CLMsgJob1;
class CLMsgJob2;
class CLMulMessage : public CLMessage//继承CLMessage的乘法消息
{
private:
	int Op1;
	int Op2;
	
public:
    	friend class CLMsgJob1;
    	friend class CLMsgJob2;
    CLMulMessage(int Op1, int Op2):CLMessage(MUL_MSG)//构造函数初始化
	{
		this->Op1 = Op1;
		this->Op2 = Op2;
	}

	virtual ~CLMulMessage()
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

class CLMsgJob1 : public CLMessageObserver//CLMsgJob继承CLMessageObserver，是线程1执行体
{
public:
	CLMsgJob1()
	{
	}

	virtual ~CLMsgJob1()
	{
	}

	virtual CLStatus Initialize(CLMessageLoopManager *pMessageLoop, void* pContext)
	{
		//注册回调函数到线程，用于绑定，可以找到具体做什么操作
		pMessageLoop->Register(MUL_MSG, (CallBackForMessageLoop)(&CLMsgJob1::Do_SubMsg));
		pMessageLoop->Register(QUIT_MSG, (CallBackForMessageLoop)(&CLMsgJob1::Do_QuitMsg));

		return CLStatus(0, 0);
	}

	CLStatus Do_SubMsg(CLMessage *pM)
	{
		CLMulMessage *pSubMsg = (CLMulMessage *)pM;

		cout <<"thread 1:"<< pSubMsg->Op1 << "x" << pSubMsg->Op2 << "=" << pSubMsg->Op1 * pSubMsg->Op2 << endl; //执行乘法操作

		return CLStatus(0, 0);
	}

	CLStatus Do_QuitMsg(CLMessage *pM)
	{
		cout << "thread 1 quit..." << endl;//退出消息

		return CLStatus(QUIT_MESSAGE_LOOP, 0);
	}
};
class CLMsgJob2 : public CLMessageObserver//CLMsgJob继承CLMessageObserver，是线程2执行体
{
public:
	CLMsgJob2()
	{
	}

	virtual ~CLMsgJob2()
	{
	}

	virtual CLStatus Initialize(CLMessageLoopManager *pMessageLoop, void* pContext)
	{
		//注册回调函数到线程，用于绑定，可以找到具体做什么操作
		pMessageLoop->Register(MUL_MSG, (CallBackForMessageLoop)(&CLMsgJob2::Do_SubMsg));
		pMessageLoop->Register(QUIT_MSG, (CallBackForMessageLoop)(&CLMsgJob2::Do_QuitMsg));

		return CLStatus(0, 0);
	}

	CLStatus Do_SubMsg(CLMessage *pM)
	{
		CLMulMessage *pSubMsg = (CLMulMessage *)pM;
		
		cout <<"thread 2:"<< pSubMsg->Op1 << "x" << pSubMsg->Op2 << "=" << pSubMsg->Op1 * pSubMsg->Op2 << endl; //执行乘法操作

		return CLStatus(0, 0);
	}

	CLStatus Do_QuitMsg(CLMessage *pM)
	{
		cout << "thread 2 quit..." << endl;//退出消息

		return CLStatus(QUIT_MESSAGE_LOOP, 0);
	}
};

int main()
{	
	CLThreadProxy proxy(new CLMsgJob1, "one_Thread", true);//库，创建线程
	
	proxy.Run(0);

	CLThreadProxy proxy1(new CLMsgJob2, "two_Thread", true);//库，创建线程
	
	proxy1.Run(0);
	cout<<"send message to 1 and 2"<<endl;
	CLExecutiveNameServer::PostExecutiveMessage("one_Thread", new CLMulMessage(2, 1));//向one_Thread线程发送消息
	CLExecutiveNameServer::PostExecutiveMessage("two_Thread", new CLMulMessage(2, 1));
	CLExecutiveNameServer::PostExecutiveMessage("one_Thread", new CLMulMessage(4, 2));
	CLExecutiveNameServer::PostExecutiveMessage("two_Thread", new CLMulMessage(4, 2));
	CLExecutiveNameServer::PostExecutiveMessage("one_Thread", new CLMulMessage(6, 3));
	CLExecutiveNameServer::PostExecutiveMessage("two_Thread", new CLMulMessage(6, 3));
	CLExecutiveNameServer::PostExecutiveMessage("one_Thread", new CLQuitMessage());

	cout<<"send message only to 2"<<endl;
	CLExecutiveNameServer::PostExecutiveMessage("two_Thread", new CLMulMessage(1, 1));//向two_Thread线程发送消息
	CLExecutiveNameServer::PostExecutiveMessage("two_Thread", new CLMulMessage(3, 2));
	CLExecutiveNameServer::PostExecutiveMessage("two_Thread", new CLMulMessage(5, 6));
	CLExecutiveNameServer::PostExecutiveMessage("two_Thread", new CLQuitMessage());

	return 0;
}
