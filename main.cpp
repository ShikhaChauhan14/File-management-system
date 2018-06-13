#include "disk.h"

void help() {
  cout << color::yellow << "mkdir directoryName : " << color::reset << " To create directory " << endl;
  cout << color::yellow << "cd directoryName : " << color::reset << " Change current working directory " << endl;
  cout << color::yellow << "cd .. : " << color::reset << " Change current directory to parent " << endl;
  cout << color::yellow << "prmdir directoryName : " << color::reset << " To delete a directory permanently" << endl;
  cout << color::yellow << "create fileName : " << color::reset << " To create a new file " << endl;
  cout << color::yellow << "rm fileName : " << color::reset << " To delete a file " << endl;
  cout << color::yellow << "restore fileName : " << color::reset << " To restore a deleted file" << endl;
  cout << color::yellow << "prm fileName : " << color::reset << " To delete a file permanently" << endl;
  cout << color::yellow << "read fileName : " << color::reset << " To read a file" << endl;
  cout << color::yellow << "show sectorNumber : " << color::reset << " To show the content of a sector" << endl;
  cout << color::yellow << "ls : " << color::reset << " To show the content of working directory" << endl;
  cout << color::yellow << "info : " << color::reset << " To show the disk info" << endl;
  cout << color::yellow << "help : " << color::reset << " Show all commands" << endl;
  cout << color::yellow << "quit : " << color::reset << " To exit" << endl;
}

vector<string> splitCommand(string input) {
  unsigned int pos = 0, prev = 0;
  vector<string> tokens;
  do {
    pos = input.find(" ", prev);
    if (pos == string::npos) break;
    string token = input.substr(prev, pos-prev);
    if (!token.empty()) {
      tokens.push_back(token);
      if (tokens.size() == 3) break;
    }
    prev = pos+1;
  } while (pos < input.length() && prev < input.length());
  return tokens;
}


int main()
{   
  rootDir mydirectory;
  char value[19];

  string input;
  cin.ignore();

  while(1) {
    cout << color::magenta << directoryName.top() << color::cyan << " ➜ " << color::reset ;
    getline(cin, input);
    vector<string> tokens = splitCommand(input);

    if(tokens.size() == 3) {
      cout << color::red << "Invalid command" << color::reset << endl;
      continue;
    }
    if(tokens.size() == 0) {
      continue;
    }

    vector <string> command = {"mkdir", "cd", "prmdir", "create", "rm", "restore", "prm", "read", "show", "ls", "info", "help", "quit"};
    
    int  x = -1;

    for(size_t i = 0; i < command.size(); i++) {
      if(tokens[0] == command[i]) {
        x = i;
        break;
      }
    }

    if(tokens.size() == 1) {
      switch(x) {
        case 9:
          mydirectory.list();
          break;

        case 10:
          mydirectory.Info();
          break;

        case 11:
          help();
          break;

        case 12:
          return 0;

        default: cout << color::red << "Invalid command" << color::reset << endl;;
      }
    }

    if(tokens.size() == 1)
      continue;
    else
      strcpy(value, tokens[1].c_str());

    switch(x)
    {
      case 0:
        if (tokens[1].size() > 19) {
          cout << color::red << "Can't make directory. Directory name can't have more than 19 characters!!" << color::reset;
          break;
        }
        mydirectory.create_directory(value);
        break;
      
      case 1:
        mydirectory.change_directory(value);
        break;

      case 2:
        mydirectory.Prm_directory(value);
        break;

      case 3:
        if (tokens[1].size() > 19) {
          cout << color::red << "Can't make file. File name can't have more than 19 characters!!" << color::reset;
          break;
        }
        mydirectory.create_file(value);
        break;

      case 4:
        mydirectory.delete_file(value);
        break;

      case 5:
        mydirectory.restore_file(value);
        break;

      case 6:
        mydirectory.Permanent_delete(value);
        break;

      case 7:
        mydirectory.read_file(value);
        break;

      case 8:
        mydirectory.display(atoi(value), 512);
        break;
      
      default:cout << color::red << "Invalid command!!" << color::reset << endl;
    }
  }
}
