/* 
 自定义的消息队列，通过单链表实现，因为需要模拟队列，即需要入队出队在链表的两端操作
 设定两个指针，头指针和尾指针，头指针端只能读，即只能出队，尾指针端只能写，即只能入队
 由于从空队列中读数据会出现异常，因此还需实现队列的判空
 */

#ifndef CLMyQueue_H
#define	CLMyQueue_H

#include <iostream>

template <class T>//T为临时数据类型，即该队列适合任意的数据类型，如基本类型和自定义的数据类型
struct CLNode		//链表节点
{
    T data;
    struct CLNode *next;
};

template <class T>
class CLMyQueue
{
private:
    struct CLNode<T> *QueueHead;//队头指针
    struct CLNode<T> *QueueTail;//队尾指针
	
public:
    CLMyQueue();
    ~CLMyQueue();
    
    void Push(T data);  //数据入队操作
    T Pop();            //数据出队操作
    bool IsEmpty();//不能从空队列读数据，因此需进行队列判空
  };


template <class T>
CLMyQueue<T>::CLMyQueue()	//构造函数中对队列进行初始化操作，队头、队尾置空
{
    QueueHead = NULL;
    QueueTail = NULL;
}

template <class T>
CLMyQueue<T>::~CLMyQueue()	//析构函数清空队列中的所有数据
{
    while (QueueHead != NULL)  //队头不为空，则清除队头，继续找队头的后继结点，删除所有的结点数据
    {
        struct CLNode<T> *t = QueueHead;
        QueueHead = QueueHead->next;
        delete t;
    }
}

template <class T>
void CLMyQueue<T>::Push(T data)//数据入队操作
{
    if (IsEmpty())//如果队列为空，将数据入队，并且队头和队尾指针都指向该数据
    {
        QueueHead = new struct CLNode<T>;
        QueueHead->data = data;
        QueueTail = QueueHead;
        return;
    }
    
	//在队尾插入数据
    struct CLNode<T> *newNode = new struct CLNode<T>;
    newNode->data = data;
    newNode->next = NULL;
    QueueTail->next = newNode;
    QueueTail = newNode;
}

template <class T>
T CLMyQueue<T>::Pop()	//数据出队操作
{
    if (IsEmpty())//不能从空队列中读数据，因此判定队列是否为空，空则返回空指针
    {
        return NULL;
    }
    
    T data = QueueHead->data;//取出对头数据，并修改队头指针
    struct CLNode<T> *t = QueueHead;
    QueueHead = QueueHead->next;
    delete t;
    return data;
}
 
template <class T>
bool CLMyQueue<T>::IsEmpty()//判断队列是否为空
{
    if (QueueHead == NULL) //当队头指针为空时，队列为空
    {
        return true;
    }
    return false;
}


#endif	

