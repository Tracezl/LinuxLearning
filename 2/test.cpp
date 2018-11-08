#include <iostream>
#include "MyFile.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;


int main()
{
	char file[10]="1.txt";
	MyFile mf=MyFile(file, 02,3);
	char buffer[20]="1234567899876543210";
	for(int i=0;i<10;i++)
		mf.Writefile(buffer);
	int pos=mf.Seekfile(0,1);
	cout<<"pos1:"<<pos<<endl;
	cout<<"after move,pos2:"<<mf.Seekfile(-100,1)<<endl;
	char but[2];
	mf.Readfile(but,2);
	//cout<<"after read,pos3:"<<mf.Seekfile(0,1)<<endl;
	cout<<"first read:"<<but[0]<<but[1]<<endl;
	char but1[2];
	mf.Readfile(but1,2);
	//cout<<"after read again,pos4:"<<mf.Seekfile(0,1)<<endl;
	cout<<"second read:"<<but1[0]<<but1[1]<<endl;
	mf.Closefile();
	return 0;
}