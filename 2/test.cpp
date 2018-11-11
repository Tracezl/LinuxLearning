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
	MyFile mf=MyFile(file, 100,3);
	char buffer[20]="qazwsxedcrfvtgbyhnu";
	for(int i=0;i<7;i++)
		mf.Writefile(buffer);
	int pos=mf.Seekfile(0,1);
	cout<<"pos1:"<<pos<<endl;
	cout<<"move,pos2:"<<mf.Seekfile(-100,1)<<endl;
	char but[4];
	mf.Readfile(but,3);
	but[3]='\0';
	cout<<"first read:"<<but<<endl;
	for(int i=0;i<5;i++)
	{
		char but1[5];
		but[4]='\0';
		mf.Readfile(but1,4);
		cout<<i<<" read:"<<but1<<endl;
	}
	mf.Closefile();
	return 0;
}