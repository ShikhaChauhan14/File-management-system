#include "disk.h"

void rootDir::create_directory(char dirName[19])
/*
  Objective: To create a directory and update it's entry in parent directory
  Input parameters: 
    dirName: name of the directory that is to be created
  Approach:
    A directory of the given name will be created only if:
    1. a directory of the given name does not already exists
    2. there is enough space for the entry of the directory in the directory
    3.there is enough space in the disk to store the directory
  Return value: None

*/
{ 
  int current_directory = directorySector.top();
  
  //If directory of the given name already exist.
  if(type(dirName, 'y')) {
    cout << "Directory already exist!!" << endl;
    return;
  }

  int size = 1024;
  int noOfSector = 2;       //total number of sectors required to store a directory is 2
  free_sectors( noOfSector);

  //Check if there is sufficient space in the disk.
  if(freeSectors.empty()) {
    cout << color::yellow << "Not enough free space in disk to store the directory" << color::reset << endl;
    return;  
  }

  for(int k = 0; k < 2; k++)
  {
    read_sector(current_directory + k);
    for(int l = 0; l < 16; l++)
    { 
      //If there exist space in the directory for the entry
      if(int(bufferSector[32*l]) == char(0))
      { 
        //Updating sector status.
        for(int i = 0; i < noOfSector; i++) {
          update_status((freeSectors[i]), freeSectors[i + 1]);
          for(int j = 0; j < buffersize; j++) { 
            bufferSector[j] = char(0);
          }
          write_sector(freeSectors[i] , bufferSector);
        }

        // Update new entry to the current directory.
        strcpy(disk.entry.name,dirName);
        disk.entry.file_size = size;
        disk.entry.start_sector = freeSectors[0];
        disk.entry.parent_dir = current_directory;
        disk.entry.is_directory = 'y';
        read_sector(current_directory + k);

        for(int m = 0; m < 32; m++)
          bufferSector[32*l + m] = disk.filebuff[m];
        write_sector((current_directory + k) , bufferSector);
          
        cout << color::green << "Directory has been created !!" << color::reset <<endl;
        return;        
      }
    } 
  }
  cout << color::yellow << "Not free space in directory" << color::reset << endl;
  return;
}

void rootDir::change_directory(char dirName[19])
/*
  Objective: To change working directory
  Input parameters: 
    dirName: name of the directory that is to be set
  Approach:
    A directory of the given name will be set as working directory only if:
    1. a directory of the given name does exists in the parent directory
  Return value: None

*/
{ 
  if(strcmp(dirName, "..") == 0) {
    if(directorySector.top() == res_sector) {
      cout << color::yellow << "Working directory is root" << color::reset << endl;
    }
    else {
      directorySector.pop();
      directoryName.pop();
    }
    return;
  }
  int current_directory = directorySector.top();
  for(int i = 0; i < 2; i++)
  {
    read_sector(current_directory + i);
    for(int j = 0; j < 16; j++)
    {
      for(int k = 0; k < 32; k++)
        disk.filebuff[k] = bufferSector[32*j+k];
      if(strcmp(disk.entry.name, dirName) == 0 && disk.entry.is_directory == 'y')
      {
        string temp = disk.entry.name;
        directoryName.push(temp);
        directorySector.push(disk.entry.start_sector);
        cout << color::green << "Directory has been set as wroking directory" << color::reset << endl;
        return;
      }
    }
  }
  cout << color::red << "No such directory exist!!" << color::reset << endl;
}

void rootDir::prm_directory(char dirName[19], int current)

/*
  Objective: To delete a directory permanently from the disk
  Input parameters: 
    dirName: name of the directory
  Approach:
    directory of the given name will be deleted from working directory only if it exist
    Delete all the files recusively
    Make status of all the child directory entry sector as free
  Return value: None

*/

{
  for(int j = 0; j < 16; j++) {
    read_sector(current);
    for(int k = 0; k < 32; k++)
      disk.filebuff[k] = bufferSector[32*j+k];
    //File
    if(disk.entry.is_directory == 'n') {
      permanent_delete(disk.entry.name, current);
    }
    //Directory
    if(disk.entry.is_directory == 'y') {
      prm_directory(disk.entry.name, disk.entry.start_sector);
    }
  }

  int next = check_status(current);
  //mark status of directory sector free
  update_status(current, 0);
  //Not end of the directory
  if(next != 1) { 
    prm_directory(dirName, next);
  }
}

void rootDir::Prm_directory(char dirName[19])
/*
  Objective: Wrapper function to call prm_directory() method and mark delete diretory entry from parent directory
  Input parameters: 
    dirName: name of the directory
  Return value: none

*/
{

  if(!type(dirName, 'y')) {
    cout << color::red << "No such directory exist!!" << color::reset  << endl;
    return;
  }

  prm_directory(dirName, disk.entry.start_sector);

  //Remove directory entry from parent;
  type(dirName, 'y');
  int parent = disk.entry.parent_dir;
  for(int i = 0; i < 2; i++) {
    read_sector(parent + i);
    for(int j = 0; j < 16; j++) {
      for(int k = 0; k < 32; k++)
        disk.filebuff[k] = bufferSector[32*j+k];
      if(strcmp(disk.entry.name, dirName) == 0 && disk.entry.is_directory == 'y') {
        for(int k = 0; k < 32; k++)
          disk.filebuff[k] = char(0);
        for(int k = 0; k < 32; k++)
          bufferSector[32*j+k] = disk.filebuff[k];
        write_sector(parent + i, bufferSector);
        break;
      }
    }
  }
  cout << "Directory has been permanently deleted!!" << endl;
}

