#include <iostream>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int main()
{
  int fd = open("a.c", O_RDWR | O_APPEND);
  if(-1 == fd)
  {
    cout << "open error" << endl;
    return 0;
  }

  char buf;// = 'm';
  if(read(fd, &buf, 1) == -1)
  { 
    cout << "read error" << endl;
    close(fd);
    return 0;
  }

  off_t CurrentPosition = lseek(fd, 0, SEEK_CUR);
  if(CurrentPosition == -1)
  {
    cout << "lseek error" << endl;
    close(fd);
    return 0;
  }

  cout << "CurrentPosition:  " << CurrentPosition << endl;
  cout << "first read:  " << buf << endl;

  if(lseek(fd, 3, SEEK_SET) == -1)
  {
    cout << "lseek error" << endl;
    close(fd);
    return 0;
  }

  off_t NewCurrentPosition = lseek(fd, 0, SEEK_CUR);
  if(NewCurrentPosition == -1)
  {
    cout << "lseek error" << endl;
    close(fd);
    return 0;
  }

  
  char buf1;
  if(read(fd, &buf1, 1) == -1)
  {
    cout << "write error" << endl;
    close(fd);    
    return 0;
  }
  cout << "NewCurrentPosition:  " << NewCurrentPosition << endl;
  cout << "second read:  " << buf1 << endl;
  close(fd);

  return 0;
}