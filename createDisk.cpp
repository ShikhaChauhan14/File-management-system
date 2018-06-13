#include "disk.h"

rootDir::rootDir()
/*
  Objective:To create a new disk if disk of the given file name doesn't exist and write 0000 in reserved sector, 0000 in the first two unreserved sectors andA,B,C,...J in the remaining unreserved sector.
  Return value: None
*/
{
  
  ifstream f;
  cout << color::cyan << "Enter disk name: " << color::reset;
  cin >> disk_name;
  f.open(disk_name, ios::binary | ios::in | ios::out);
  bool createNew = (f.peek() == ifstream::traits_type::eof());
  f.close();

  if(createNew){  
    cout << color::cyan << "Enter disk size(any integer) in MB: " << color::reset;
    cin >> disk_size;
    
    while(cin.fail()) {
      cin.clear();
      cin.ignore();
      cout << color::cyan << " Enter an integer : " << color::reset;
      cin >> disk_size;
    }
    total_sector = (disk_size*pow(2,20))/pow(2,9);
    res_sector = total_sector/pow(2,7);
    res_write();
    rootDir_write();
    unres_write();
    update_status(res_sector , res_sector + 1);
    update_status(res_sector + 1, 1);
    cout << color::green << "Disk " << disk_name << " has been created !!" <<color::reset << endl;
  }
  else {
    int size;
    cout << color::yellow << "Disk already exists!!" << color :: reset << endl;
    ifstream fp;
    fp.open(disk_name, ios::in|ios::binary);
    fp.seekg(0, ios::end);
    size = fp.tellg();
    disk_size = size/pow(2,20);
    total_sector = (disk_size*pow(2,20))/pow(2,9);
    res_sector = total_sector/pow(2,7);
  }
      //directoryName.push(res_sector);
      directorySector.push(res_sector);               // push root directory starting sector
      directoryName.push("~");
      cout << "Total number of sectors in " << disk_size << " MB disk: " << total_sector << endl;
      cout << "Total number of reserved sectors in " << disk_size << " MB disk: " << res_sector << endl;

}