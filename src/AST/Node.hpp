#pragma once

#include <iostream>
using namespace std;

class Node {
public:
  virtual bool type_checking() {
    return false;
  }

  virtual void print(ostream& os) {
    os << int(this) << "[label = \"Node\"]" << endl;
  }
};
