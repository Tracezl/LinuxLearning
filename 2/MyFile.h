#pragma once
#ifndef MYFILE
#define MYFILE 
#define CREATE O_CREAT
#define READ   O_RDONLY
#define WRITE  O_WRONLY
#define RW     O_RDWR
#define EXIST  O_EXCL|O_CREAT 
#define APPEND O_APPEND
#define SET 0  //������ת����ʼ��
#define CUR 1
#define END 2
#endif
class MyFile
{
private: int  fd;
		 unsigned int CurrentPosition;
		 unsigned int writeNowPosition;
		 unsigned int readNowPosition;
		 unsigned int readLength;
		 bool readBuffer=false;
		 char *m_Buffer;
		 char *m_readBuffer;
public:
	MyFile(char *filename, int oflag, int mode);
	~MyFile();
	
	int Readfile(char * buffer, int nbytes);
	int Writefile(char * buffer);
	int Seekfile(int offset, int whence);
	void readBufferUnUse();
	int Flash();
	int Closefile();

};

