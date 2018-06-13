#include "disk.h"

void rootDir::free_sectors(int num)
/*
  Objective: to store sector number of free sectors in vector freeSectors;
  input parameter:
    num: number of free sectors
  Return value: None
*/
{
  int j = 0;
  freeSectors.clear();
  for(int i = 0; i < (total_sector-res_sector-2); i++) { 
    if(check_status(res_sector + 2 + i) == 0) { 
      freeSectors.push_back(res_sector + 2 + i);
      j++;
    }
    if(j == num) {
      freeSectors[num] = 1;
      return;
    }
  }
  freeSectors.clear();
  return;
}

int rootDir::check_status(int index)
/*
    Objective:to check wheather a sector is reserved, root directory, free or busy.
    Approach: check status using union.
    Input Parameter:
        index: Sector number
    Return value:
      status of the sector
     
*/
{   
  if(0 <= index && index < res_sector)
    return 2;
  else if (index == res_sector)
    return res_sector + 1;
  else if (index == res_sector + 1)
    return 1;

  fstream f;
  int sector_pos , byte_pos;
  sector_pos = sector_number(index);
  byte_pos = byte_number(index);
  f.open(disk_name,ios::in|ios::binary);
  f.seekg(sector_pos*buffersize);
  f.read(bufferSector,buffersize);
  f.close();
  for(int i = 0; i < 4; i++)
    d.buffer[i] = bufferSector[byte_pos+i];
  return d.num;
}

int rootDir::sector_number(int index)
/*
  Objective: to get sector number where the status of the given sector stored in reserved sector.
  input paramete: 
    indx: sector number
  Return Value:
    sector number where the status stored
*/
{
  int num = (index - res_sector)/128;
  return num;
}

int rootDir::byte_number(int index)
/*
  Objective: to get byte number where the status of the given sector stored in reserved sector.
  input paramete: 
    indx: sector number
  Return Value:
    byte number where the status is stored
*/
{
  int num = ((index-res_sector)%128)*4;
  return num;
}

void rootDir::write_sector(int index , char bufferSector[512])
/*
    Objective: to write in a free sector byte by byte
    Input parameter:
        index:sector number
        a: character pointer to an array that is to be write in the sector.
  Return value: None
*/
{   
  fstream f;
  f.open(disk_name,ios::out|ios::in|ios::binary);
  
  f.seekp((index)*buffersize);
  f.write(bufferSector,buffersize);
  
  f.close();
  return;
}

void rootDir::update_status(int index, int num)
/*
  Objective: To update status of a sector to given integer num
  Input parameter:
    index: enter sector number
    num: value that is to be write in the status
  Return value: None
*/
{
  fstream f;
  d.num = num;
  f.open(disk_name,ios::in|ios::out|ios::binary);
  f.seekg((index - res_sector) / 128 * buffersize);
  f.read(bufferSector,buffersize);
  for(int i = 0;i < 4; i++)
    bufferSector[((index - res_sector) % 128) * 4 + i] = d.buffer[i];
  f.seekp((index - res_sector) / 128 * buffersize);
  f.write(bufferSector, buffersize);
  f.close();
  return;
}


void rootDir::read_sector(int index)        //function read whose return type is character pointer.
/*
    Objective: to read the content of a sector byte by byte
    Input parameter:
        index:sector number
    Return value: None
*/
{   fstream f;
  f.open(disk_name,ios::in|ios::binary);
  f.seekg((index) * buffersize);
  f.read(bufferSector,buffersize);
  f.close();
  return;
}

void rootDir::display(int index, int size)
/*
    Objective: to read the content of a sector byte by byte
    Input parameter:
        index:sector number
    Return value: None
*/
{
  
  if(index < 0 || index > total_sector) {
    cout << "Sector index out of range!!" << endl;
    return;
  }
  if(index == res_sector || index == res_sector + 1) {
    cout << "Root directory!!" << endl;
    return;
  }
  int temp = check_status(index);
  read_sector(index);
  
  if(temp == 2)
  { 
    for( int i = 0; i < buffersize; i += 4)
    {
      for(int j = 0; j < 4; j++)
        d.buffer[j] = bufferSector[i + j] ;
      cout << d.num << " ";
    }
  cout << endl;
  }
  
  for(int i = 0; i < size; i++)
    cout << bufferSector[i];
  cout << endl;
  return;
}

bool rootDir::type(char name[19], char t)
/*
    Objective: to check wheather file/directory exist of the given name.
    Input parameter:
        name: file/directory name
        t: 
        'y' if directory 
        'n' if file 
    Return value: true if entry of the given type exist else false
*/
{
  int current_directory = directorySector.top();
  for(int i = 0; i < 2; i++) {
    read_sector(current_directory + i);
    for(int j = 0; j < 16; j++) {
      for(int k = 0; k < 32; k++)
        disk.filebuff[k] = bufferSector[32*j+k];
      if(strcmp(disk.entry.name, name) == 0 && disk.entry.is_directory == t) 
        return true;
    }
  }
  return false; 
}

void rootDir::list()
/*
  Objective: To give list of all entries in the current working directory.
  Input parameters: 
    None
  Return value: None
*/
{
  int current_directory = directorySector.top();
  cout << setw(19)<< "Name" << setw(10) << "Type"<<setw(10) <<"Size" << endl;
  for(int i = 0; i < 2; i++) {
    read_sector(current_directory + i);
    for(int j = 0; j < 16; j++) {
      for(int k = 0; k < 32; k++)
        disk.filebuff[k] = bufferSector[32*j+k];
      if(disk.entry.start_sector > 0 && disk.entry.is_directory == 'y') {
        cout << color::green << setw(19) << disk.entry.name << color::reset << setw(10) << "Dir" << setw(10) << disk.entry.file_size << endl;
      }
      if(disk.entry.start_sector > 0 && disk.entry.is_directory == 'n') {
        cout << setw(19) << disk.entry.name << setw(10) << "file" << setw(10) << disk.entry.file_size << endl;
      }
    }
  } 
}

void rootDir::info(int current, int t)
/*
  Objective: To give the hierarchical structure of the disk
  Input parameters: 
    current
    t: hierarchical level
  Return value: None

*/
{
  for(int j = 0; j < 16; j++) {
    read_sector(current);
    for(int k = 0; k < 32; k++)
      disk.filebuff[k] = bufferSector[32*j+k];
    if(disk.entry.start_sector > 0 && disk.entry.is_directory == 'n') {
      for(int l = 0; l < t; l++)
        cout << "\t";
      cout << disk.entry.name << endl;
    }
    if(disk.entry.start_sector > 0 && disk.entry.is_directory == 'y') {
      for(int l = 0; l < t; l++)
        cout << "\t";
      cout << color::green << disk.entry.name << color::reset << endl;
      info(disk.entry.start_sector, t + 1);
    }
  }
  if(check_status(current) != 1) {
    current = check_status(current); 
    info(current , t);
  }
}

void rootDir::Info()
// Wrapper function to call info method
{
  info(res_sector, 1);
}

