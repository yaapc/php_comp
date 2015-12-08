#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include<fstream>

using namespace std;

class Logger {
public:
  Logger(string file_name)  {
    os.open(file_name.c_str(), ofstream::out);
  }

  ~Logger() {
	  os.close();
  }

  void log(string s, bool new_line = true) {
	  os<<s;
    if (new_line) os<<endl;
    else os<<" ";
  }

  void log(int i, bool new_line = true) {
    if (i > 0 && i < 256)
      os<<char(i);
	  else
      os<<i;
    if (new_line) os<<endl;
    else os<<" ";
  }

  void log(double d, bool new_line = true) {
	  os<<d;
    if (new_line) os<<endl;
    else os<<" ";
  }
private:
  ofstream os;
};
#endif