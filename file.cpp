#include "disk.h"

void rootDir::create_file(char filename[19])
/*
  Objective: To create a file and update it's entry in directory
  Input parameters: 
    filename: name of the file that is to be created
  Approach:
    A file of the given name will be created only if:
    1. a file of the given name does not already exists
    2. there is enough space for the entry of the file in the directory
    3.there is enough space in the disk to store the file
  Return value: None
*/
{ 
  int current_directory = directorySector.top();

  //If file of the given name already exist.
  if(type(filename, 'n')) {
    cout << "File already exist!!" << endl;
    return;
  }

  int size;
  char a;
  cout << color::cyan << "Enter file size : " << color::reset;
  cin >> size;
  while(cin.fail()) {
    cin.clear();
    cin.ignore();
    cout << color::cyan << " Enter an integer " << color::reset;
    cin >> size;
  }
  cout << color::cyan << "Enter character that is to be write in the file: " << color::reset;
  cin >> a;
  cin.ignore();

  int noOfSector = (size - 1)/512 + 1;       //total number of sectors required to store file
  free_sectors( noOfSector);

  //Check if there is sufficient space in the disk.
  if(freeSectors.empty()) {
    cout << color::yellow << "Not enough free space in disk to store the file" << color::reset << endl;
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
        for( int i = 0; i < noOfSector; i++) {
          update_status((freeSectors[i]), freeSectors[i + 1]);
          for(int j = 0; j < buffersize; j++)
          { 
            if((512 * i + j) < size)
              bufferSector[j] = a;
            else
              bufferSector[j] = char(' ');
          }
          write_sector(freeSectors[i] , bufferSector);
        }

        // Update new entry to the current directory.
        strcpy(disk.entry.name,filename);
        disk.entry.file_size = size;
        disk.entry.start_sector = freeSectors[0];
        disk.entry.parent_dir = current_directory;
        disk.entry.is_directory = 'n';
        read_sector(current_directory + k);

        for(int m = 0; m < 32; m++)
          bufferSector[32*l + m] = disk.filebuff[m];
        write_sector((current_directory + k) , bufferSector);
        cout <<"file has been created !!" << endl;
        return;
      }
    } 
  }
  cout << "Not free space in directory" << endl;
  return;
}


void rootDir::read_file(char filename[19])
/*
  Objective:To display the content of a given file.
  Input parameter:
   filename : the name of the file to be displayed
  Return value: None
*/
{
  int current_directory = directorySector.top();
  for(int i = 0; i < 2; i++) {
    read_sector(current_directory + i);
    for(int j = 0; j < 16; j++) {
      for(int k = 0; k < 32; k++)
        disk.filebuff[k] = bufferSector[32*j+k];
      if(strcmp(disk.entry.name, filename) == 0 && disk.entry.is_directory == 'n') {
        int size = disk.entry.file_size;

        if(check_status(disk.entry.start_sector) < 0) {
          cout << "Deleted file!! To read deleted file first restore it!!" << endl;
          return;
        }
        int temp = disk.entry.start_sector;
        do { 
          if(size >= buffersize) { 
            display(temp, buffersize);
            temp = check_status(temp);
            size -= buffersize;
          }
          else {
            display(temp, size );
            temp = check_status(temp);
          }

        }while(temp != 1);
        return;
      }
      read_sector(current_directory + i);
    }
  }
  cout << "file doesn't exist !!" << endl;
  return;
}

void rootDir::file_info(char filename[19]) {
  int current_directory = directorySector.top();
  for(int i = 0; i < 2; i++) {
    read_sector(current_directory + i);
    for(int j = 0; j < 16; j++) {
      for(int k = 0; k < 32; k++)
        disk.filebuff[k] = bufferSector[32*j+k];
      if(strcmp(disk.entry.name, filename) == 0 && disk.entry.is_directory == 'n') {
        cout << "filename: " << disk.entry.name;
        cout << "starts at: " << disk.entry.start_sector;
      }
      else
        cout << "File doesn't exist!!" << endl;
      return;
    }
  }
}


void rootDir::delete_file(char filename[19])
/*
  Objective: To mark a file as deleted.
  Input parameter:
    filename: name of the file
  Approach:
    changing the sector status of each sector allotted to a given file as negative
  Return value: None  
*/
{
  int current_directory = directorySector.top();
  for(int i = 0; i < 2; i++) {
    read_sector(current_directory + i);
    for(int j = 0; j < 16; j++) {
      for(int k = 0; k < 32; k++)
        disk.filebuff[k] = bufferSector[32*j+k];
      if(strcmp(disk.entry.name, filename) == 0 && disk.entry.is_directory == 'n') { 
        if(check_status(disk.entry.start_sector) > 0) { 
          int temp = disk.entry.start_sector;
          int num = check_status(temp);
          do { 
            update_status(temp, (-1)*num);
            temp = (-1)*check_status(temp);
            num = check_status(temp);
          }while(temp != 1);
          cout << "File has been deleted!!" << endl;
          return;
        }
        else
          cout << "file is already deleted!!" << endl; 
        return;
      }
    }
  }
  cout << "file doesn't exist !!"<<endl;
  return;
}

void rootDir::restore_file(char filename[19])
/*
  Objective: To restore a deleted file.
  Input parameter:
    filename: name of the file
  Approach:
    changing the sector status of each sector allotted to a given file as positive, if the file is deleted
  Return value: None  
*/
{
  int current_directory = directorySector.top();
  for(int i = 0; i < 2; i++) {
    read_sector(current_directory + i);
    for(int j = 0; j < 16; j++) {
      for(int k = 0; k < 32; k++)
        disk.filebuff[k] = bufferSector[32*j+k];
      if(strcmp(disk.entry.name, filename) == 0 && disk.entry.is_directory == 'n') { 
        if(check_status(disk.entry.start_sector) <  0) { 
          int temp = disk.entry.start_sector;
          int num = check_status(temp);
          do
          {
            update_status(temp, (-1)*num);
            temp = check_status(temp);
            num = check_status(temp);
          }while(temp != 1);
          cout << "File has been restored!!" << endl;
          return;
        }
        else
          cout << "File exist in the directory!!" << endl;
        return;
        read_sector(current_directory + i);
      }
    }
  }
  cout << "file doesn't exist !!"<<endl;
  return;
}

bool rootDir::permanent_delete(char filename[19], int current_directory)
/*
  Objective: To delete a file permanently from the disk.
  Input parameter:
    filename: name of the file.
    current_directory: Parent directory of file.
  Approach:
    Removing the file entry from the root directory 
    changing the sector status of each sector allotted to a given file as 0, if the file is deleted or existing.
  Return value: None  
*/
{
  for(int i = 0; i < 2; i++) {
    read_sector(current_directory + i);
    for(int j = 0; j < 16; j++) {
      for(int k = 0; k < 32; k++)
        disk.filebuff[k] = bufferSector[32*j+k];
      if(strcmp(disk.entry.name, filename) == 0  && disk.entry.is_directory == 'n') { 
        if(check_status(disk.entry.start_sector) > 0) {
          int temp = disk.entry.start_sector;
          int temp2 = check_status(temp);
          do
          { 
            update_status(temp, 0);
            temp = temp2;
            temp2 = check_status(temp);
          }while(temp != 1);
          
        }
        else {
          int temp = disk.entry.start_sector;
          int temp2 = (-1)*check_status(temp);
          do
          { 
            update_status(temp, 0);
            temp = temp2;
            temp2 = (-1)*check_status(temp);
          }while(temp != 1);  
        }
        read_sector(current_directory + i);
        for(int k = 0; k < 32; k++)
          disk.filebuff[k] = char(0);
        for(int k = 0; k < 32; k++)
          bufferSector[32*j+k] = disk.filebuff[k];

        write_sector(current_directory + i, bufferSector);

        return true; 
      } 
    }
  }
  return false;
}

void rootDir::Permanent_delete(char filename[19])
/*
  Objective: Wrapper function to call permanent_delete method.  
*/
{
  int current_directory = directorySector.top();
  if(permanent_delete(filename, current_directory)) 
    cout << "File has been permanently deleted!!" << endl;
  else
    cout << color::red << "file doesn't exist !!" << color::reset <<endl;
}
