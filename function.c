#include "function.h"
#include <math.h>

void delete(void *ptr)
{
  if(ptr != NULL)
    free(ptr);

  ptr = NULL;
}

double dbilinear(double *p, double x, double y, int width, int height)
{
  int i = floor(x);
  int j = floor(y);
  double dx = x - i;
  double dy = y - j;
  int index = i*height + j;
  return p[index]*(1.0-dx)*(1.0-dy)
       + p[index+height]*dx*(1.0-dy)
       + p[index+1]*(1.0-dx)*dy
       + p[index+height+1]*dx*dy;
}

FILE* openFile(const char* file_name)
{
  FILE *fp;
  if( (fp=fopen(file_name, "w") ) == NULL )
  {
    printf("cannot open file %s \n", file_name);
    exit(2);
  }
  return fp;
}

FILE* FileOpen(const char* file_name, const char* mode)
{
  FILE *fp;
  if( (fp=fopen(file_name, mode) ) == NULL )
  {
    printf("cannot open file %s \n", file_name);
    exit(2);
  }
  return fp;
}

#if defined(MAC_OS) || defined(LINUX_OS)
#include <sys/stat.h>
#include <unistd.h>

bool makeDirectory(const char* name)
{
  if(mkdir(name,   S_IRUSR | S_IWUSR | S_IXUSR |         /* rwx */
    S_IRGRP | S_IWGRP | S_IXGRP |         /* rwx */
           S_IROTH | S_IXOTH | S_IXOTH ) == 0)
  {
    return true;
  } else {
    //作れなかったときは、多分すでに存在するとき.
    return false;
  }
}

void moveDirectory(const char* name)
{
  if(chdir(name)==0)
  {
    printf("move to %s\n",name);
  } else {
    printf("cannot move to %s\n",name);
    exit(2);
  }
}

void makeAndMoveDirectory(const char* name)
{
  makeDirectory(name);
  moveDirectory(name);
}
#endif

