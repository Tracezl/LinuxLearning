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

#define SUB_MSG 0  //������Ϣ��ʶ��
#define QUIT_MSG 1 //�˳���Ϣ��ʶ��

class CLMsgJob;

class CLSUBMessage : public CLMessage//�̳�CLMessage�ļ�����Ϣ
{
private:
	int Op1;
	int Op2;
	
public:
    	friend class CLMsgJob;
    CLSUBMessage(int Op1, int Op2):CLMessage(SUB_MSG)//���캯����ʼ��
	{
		this->Op1 = Op1;
		this->Op2 = Op2;
	}

	virtual ~CLSUBMessage()
	{
	}
};

class CLQuitMessage : public CLMessage  //�̳�CLMessage���˳���Ϣ
{
public:
    CLQuitMessage() : CLMessage(QUIT_MSG)
    {
	}

	virtual ~CLQuitMessage()
	{
	}
};

class CLMsgJob : public CLMessageObserver//CLMsgJob�̳�CLMessageObserver�����߳�ִ����
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
		//ע��ص��������̣߳����ڰ󶨣������ҵ�������ʲô����
		pMessageLoop->Register(SUB_MSG, (CallBackForMessageLoop)(&CLMsgJob::Do_SubMsg));
		pMessageLoop->Register(QUIT_MSG, (CallBackForMessageLoop)(&CLMsgJob::Do_QuitMsg));

		return CLStatus(0, 0);
	}

	CLStatus Do_SubMsg(CLMessage *pM)
	{
		CLSUBMessage *pSubMsg = (CLSUBMessage *)pM;
		
		cout << pSubMsg->Op1 << "-" << pSubMsg->Op2 << "=" << pSubMsg->Op1 - pSubMsg->Op2 << endl; //ִ�м�������

		return CLStatus(0, 0);
	}	

	CLStatus Do_QuitMsg(CLMessage *pM)
	{
		cout << "quit..." << endl;//�˳���Ϣ
		
		return CLStatus(QUIT_MESSAGE_LOOP, 0);
	}
};

int main()
{	
	CLThreadProxy proxy(new CLMsgJob, "son_Thread", true);//�⣬�����߳�
	
	proxy.Run(0);

	CLExecutiveNameServer::PostExecutiveMessage("son_Thread", new CLSUBMessage(7, 1));//��son_Thread�̷߳�����Ϣ
	CLExecutiveNameServer::PostExecutiveMessage("son_Thread", new CLSUBMessage(9, 2));
	CLExecutiveNameServer::PostExecutiveMessage("son_Thread", new CLSUBMessage(2, 6));
	CLExecutiveNameServer::PostExecutiveMessage("son_Thread", new CLQuitMessage());

	return 0;
}
