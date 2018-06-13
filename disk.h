#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stack>
#include <vector>
#include <iomanip>
using namespace std;

class rootDir
{ 
  char disk_name[19];
  int disk_size;                           // file size
  int res_sector;                         //no of sectors that are reserved to store status of sectors.
  int total_sector;                       //Total number of sectors
  const int buffersize = 512;               //size of one sector
  
  vector<int> freeSectors;        //array to store sector number of free sectors
  int current_directory;          // current directory where a entry is to be created
  struct sectorStatus;          // structure to handle status of a sector
  struct detail;           // structure to represent the attribute of a entry in the directory
  struct status;            // structure to handle file entry in the directory 
  

  public:
    rootDir();
    void res_write();
    void rootDir_write();
    void unres_write();
    void create_file(char filename[19]);
    void create_directory(char dirName[19]);
    void change_directory(char dirName[19]);
    void prm_directory(char dirName[19], int current);
    void Prm_directory(char dirName[19]);
    void info(int current, int t);
    void Info();
    void list(); 
    bool type(char name[19], char t);                        
    void file_info(char filename[19]);
    void read_file(char filename[19]);
    void delete_file(char filename[19]);
    void restore_file(char filename[19]);
    bool permanent_delete(char filename[19], int current);
    void Permanent_delete(char filename[19]);
    int check_status(int index);          // int index: sector number
    void update_status(int index, int temp);      // int index: sector number
    void write_sector(int index, char bufferSector[512]);   // int index: sector number  char a: character to be inserted
    void read_sector(int index);            // int index: sector number
    int sector_number(int index);         // int index: sector number
    int byte_number(int index);         // int index: sector number
    void display(int index, int size);          // int index: sector number
    void free_sectors(int num);         // int num: number of free sectors
};


typedef struct {
  union{
    int num;
    char buffer[4];
  };
}sectorStatus;

struct detail {
  char name[19];
  char is_directory;      // y if directory else n.
  int start_sector;
  int file_size;
  int parent_dir;
};  

struct status {
  union{
    detail entry;
    char filebuff[32]; 
  };
};

namespace color {
  const std::string red("\033[0;31m");
  const std::string green("\033[1;32m");
  const std::string yellow("\033[1;33m");
  const std::string cyan("\033[0;36m");
  const std::string magenta("\033[0;35m");
  const std::string reset("\033[0m");
}

extern char bufferSector[512];
extern char bufferIn[512];
extern sectorStatus d;           //object of sectorStatus structure .
extern status disk;        //object of status structure .
extern stack <string> directoryName;
extern stack <int> directorySector;

