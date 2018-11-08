/* 
 �Զ������Ϣ���У�ͨ��������ʵ�֣���Ϊ��Ҫģ����У�����Ҫ��ӳ�������������˲���
 �趨����ָ�룬ͷָ���βָ�룬ͷָ���ֻ�ܶ�����ֻ�ܳ��ӣ�βָ���ֻ��д����ֻ�����
 ���ڴӿն����ж����ݻ�����쳣����˻���ʵ�ֶ��е��п�
 */

#ifndef CLMyQueue_H
#define	CLMyQueue_H

#include <iostream>

template <class T>//TΪ��ʱ�������ͣ����ö����ʺ�������������ͣ���������ͺ��Զ������������
struct CLNode		//����ڵ�
{
    T data;
    struct CLNode *next;
};

template <class T>
class CLMyQueue
{
private:
    struct CLNode<T> *QueueHead;//��ͷָ��
    struct CLNode<T> *QueueTail;//��βָ��
	
public:
    CLMyQueue();
    ~CLMyQueue();
    
    void Push(T data);  //������Ӳ���
    T Pop();            //���ݳ��Ӳ���
    bool IsEmpty();//���ܴӿն��ж����ݣ��������ж����п�
  };


template <class T>
CLMyQueue<T>::CLMyQueue()	//���캯���жԶ��н��г�ʼ����������ͷ����β�ÿ�
{
    QueueHead = NULL;
    QueueTail = NULL;
}

template <class T>
CLMyQueue<T>::~CLMyQueue()	//����������ն����е���������
{
    while (QueueHead != NULL)  //��ͷ��Ϊ�գ��������ͷ�������Ҷ�ͷ�ĺ�̽�㣬ɾ�����еĽ������
    {
        struct CLNode<T> *t = QueueHead;
        QueueHead = QueueHead->next;
        delete t;
    }
}

template <class T>
void CLMyQueue<T>::Push(T data)//������Ӳ���
{
    if (IsEmpty())//�������Ϊ�գ���������ӣ����Ҷ�ͷ�Ͷ�βָ�붼ָ�������
    {
        QueueHead = new struct CLNode<T>;
        QueueHead->data = data;
        QueueTail = QueueHead;
        return;
    }
    
	//�ڶ�β��������
    struct CLNode<T> *newNode = new struct CLNode<T>;
    newNode->data = data;
    newNode->next = NULL;
    QueueTail->next = newNode;
    QueueTail = newNode;
}

template <class T>
T CLMyQueue<T>::Pop()	//���ݳ��Ӳ���
{
    if (IsEmpty())//���ܴӿն����ж����ݣ�����ж������Ƿ�Ϊ�գ����򷵻ؿ�ָ��
    {
        return NULL;
    }
    
    T data = QueueHead->data;//ȡ����ͷ���ݣ����޸Ķ�ͷָ��
    struct CLNode<T> *t = QueueHead;
    QueueHead = QueueHead->next;
    delete t;
    return data;
}
 
template <class T>
bool CLMyQueue<T>::IsEmpty()//�ж϶����Ƿ�Ϊ��
{
    if (QueueHead == NULL) //����ͷָ��Ϊ��ʱ������Ϊ��
    {
        return true;
    }
    return false;
}


#endif	

