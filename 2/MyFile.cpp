#include "MyFile.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define BUFFER_SIZE_WRITE_FILE 4096
#define BUFFER_SIZE_READ_FILE 100
using namespace std;
MyFile::MyFile(char * filename, int oflag, int mode)
{
	if (oflag == CREATE)//权限设置mode只有在创建文件时有效
		fd = open(filename, oflag, mode);
	else
		fd = open(filename, oflag);
	if (fd == -1)
		cout << "open file error\n";
	CurrentPosition= lseek(fd, 0, SEEK_CUR);
	m_Buffer = new char[BUFFER_SIZE_WRITE_FILE];
	m_readBuffer = new char[BUFFER_SIZE_READ_FILE];
	writeNowPosition=0;
	readNowPosition=0;
}

MyFile::~MyFile()
{

}
void MyFile::readBufferUnUse()//设置readBuffer不可用状态
{
	readBuffer=false;
	readNowPosition=0;
}

int MyFile::Readfile(char * buffer, int nbytes)//读取文件
{
	Flash();
	int n;
	if (fd < 2)//文件描述符＜2表示没打开文件
	{
		cerr << "file is not open\n";
		return -1;
	}
	//如果readBuffer不可用或者读取长度大于Buffer剩余长度时，从文件中读取
	if (!readBuffer||nbytes > readLength|| nbytes + readNowPosition > readLength)
	{
		//文件中读取到返回的结果中
		if ((n = read(fd, buffer, nbytes)) < 0)
		{
			cerr << "read error\n";
			return -1;
		}
		//文件中读取到预读缓存区中
		if ((readLength=read(fd, m_readBuffer, BUFFER_SIZE_READ_FILE)) < 0)
		{
			cerr << "read error\n";
			return -1;
		}
		//将标志位还原到实际读到的地方
		CurrentPosition = lseek(fd, -readLength, SEEK_CUR);
		if (CurrentPosition < 0)
		{
			cout << "seek error\n";
			return -1;
		}
		readBuffer=true;
		cout<<"read from file"<<endl;
		readNowPosition=0;
		return n;
	}
	memcpy( buffer,m_readBuffer + readNowPosition, nbytes);
	readNowPosition += nbytes;
	CurrentPosition = lseek(fd, nbytes, SEEK_CUR);
	cout<<"read from Cache"<<endl;
	if (CurrentPosition < 0)
	{
		cout << "seek error\n";
		return -1;
	}
	return nbytes;
}
//写入数据
int MyFile::Writefile(char * buffer)
{
	if (buffer == 0)
		return -1;
	if (strlen(buffer) == 0)
		return -1;
	if (m_Buffer == 0)
		return -1;
	//写的时候需要把readBuffer设置为不可用状态
	readBufferUnUse();
	unsigned int len_strmsg = strlen(buffer);
	//写的总长超过Buffer的长度
	if (len_strmsg > BUFFER_SIZE_WRITE_FILE)
	{
		if (Flash() < 0)
			return -1;

		ssize_t r = write(fd, buffer, len_strmsg);
		if (r == -1)
			return -1;
		return  len_strmsg;
	}
	//Buffer剩下的长度小于当前写的长度
	if(len_strmsg + writeNowPosition > BUFFER_SIZE_WRITE_FILE)
	{
		if (Flash() < 0)
			return -1;
	}
	//写入缓存区
	memcpy(m_Buffer + writeNowPosition, buffer, len_strmsg);
	writeNowPosition += len_strmsg;
	cout<<"write"<<endl;
	return len_strmsg;
}

int MyFile::Seekfile(int offset, int whence)
{
	//移位时自动刷新缓存区，后续可扩展在缓存区内移位操作
	Flash();
	readBufferUnUse();
	if (fd < 2)
	{
		cout << "file is not open\n";
		return -1;
	}
	CurrentPosition = lseek(fd, offset, whence);
	if (CurrentPosition < 0)
	{
		cout << "seek error\n";
		return -1;
	}
	return CurrentPosition;
}
//将缓存区写入文件
int MyFile::Flash()
{
	if (fd == -1)
		return -1;

	if (m_Buffer == 0)
		return -1;

	if (writeNowPosition == 0)
		return 0;
	cout<<"write to file "<<endl;
	ssize_t r = write(fd, m_Buffer, writeNowPosition);
	if (r == -1)
		return -1;
	CurrentPosition += writeNowPosition;
	writeNowPosition = 0;
	return 0;
}

int MyFile::Closefile()
{
	Flash();
	return close(fd);
}
