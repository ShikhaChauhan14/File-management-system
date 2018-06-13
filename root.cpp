#include "disk.h"

char bufferSector[512];
char bufferIn[512];
sectorStatus d;       //object of sectorStatus structure .
status disk;      //object of status structure .
stack <int> directorySector;
stack <string> directoryName;

void rootDir::res_write()
 /*Objective:function to write 0000 in all the reserved sectors.
    Approach:using union
   Return value: None
 */
{ fstream f;
  f.open(disk_name, ios::out|ios::binary);
  d.num=0;
  for(int i=0; i < total_sector; i++)
    f.write(d.buffer, 4);
  f.close();
  return ;
}

void rootDir::rootDir_write()
 /*
  Objective:function to write 0000 in the first two unreserved sectors.
    Approach:using union
    Return value: None
 */
{ fstream f;
  f.open(disk_name, ios::out|ios::in|ios::binary);
  f.seekp((res_sector) * buffersize);
  d.num = 0;
  for(int i = 0; i < 32; i++)
    f.write(d.buffer, 32);
  f.close();
  return ;
}

void rootDir::unres_write()
/*
  Objective:Initialise the disk, store A, B,..., J in empty sectors.
  Return value: None
*/
{
  fstream f;
  int a = 65;                                 //65: ASCII value of character A
  f.open(disk_name, ios::in|ios::out|ios::binary);
  f.seekp((res_sector + 2) * buffersize);
  for(int i = 0; i < (total_sector - res_sector - 2); i++)
  { 
    for(int j = 0; j < buffersize; j++)
      bufferSector[j] = char(a+i%10);
    f.write(bufferSector, buffersize);
  }
  f.close();
  return;
}
