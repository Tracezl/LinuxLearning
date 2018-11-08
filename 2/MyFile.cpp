#include "MyFile.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//#define CREATE  100
#define BUFFER_SIZE_WRITE_FILE 4096
#define BUFFER_SIZE_READ_FILE 10
using namespace std;
MyFile::MyFile(char * filename, int oflag, int mode)
{
	if (oflag == CREATE)//Ȩ������modeֻ���ڴ����ļ�ʱ��Ч
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
void MyFile::readBufferUnUse()
{
	readBuffer=false;
	readNowPosition=0;
}

int MyFile::Readfile(char * buffer, int nbytes)
{
	Flash();
	int n;
	if (fd < 2)//�ļ���������2��ʾû���ļ�
	{
		cerr << "file is not open\n";
		return -1;
	}
	if (!readBuffer||nbytes > readLength|| nbytes + readNowPosition > readLength)
	{
		if ((n = read(fd, buffer, nbytes)) < 0)
		{
			cerr << "read error\n";
			return -1;
		}
		if ((readLength=read(fd, m_readBuffer, BUFFER_SIZE_READ_FILE)) < 0)
		{
			cerr << "read error\n";
			return -1;
		}
		CurrentPosition = lseek(fd, -readLength, SEEK_CUR);
		if (CurrentPosition < 0)
		{
			cout << "seek error\n";
			return -1;
		}
		readBuffer=true;
		//cout<<readBuffer<<endl;
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

int MyFile::Writefile(char * buffer)
{
	if (buffer == 0)
		return -1;
	if (strlen(buffer) == 0)
		return -1;
	if (m_Buffer == 0)
		return -1;
	//unsigned int nleftroom = BUFFER_SIZE_LOG_FILE - readNowPosition;
	readBufferUnUse();
	unsigned int len_strmsg = strlen(buffer);
	if (len_strmsg > BUFFER_SIZE_WRITE_FILE)
	{
		if (Flash() < 0)
			return -1;

		ssize_t r = write(fd, buffer, len_strmsg);
		if (r == -1)
			return -1;
		return  len_strmsg;
	}
	if(len_strmsg + writeNowPosition > BUFFER_SIZE_WRITE_FILE)
	{
		if (Flash() < 0)
			return -1;
	}
	memcpy(m_Buffer + writeNowPosition, buffer, len_strmsg);
	writeNowPosition += len_strmsg;
	return len_strmsg;
}

int MyFile::Seekfile(int offset, int whence)
{
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

int MyFile::Flash()
{
	if (fd == -1)
		return -1;

	if (m_Buffer == 0)
		return -1;

	if (writeNowPosition == 0)
		return 0;
	cout<<"xie rushuju "<<endl;
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
