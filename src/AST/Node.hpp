#pragma once

#include <iostream>
#include "../TypeSystem/TypesTable.h"
using namespace std;

class Node {
public:
  virtual bool type_checking() {
	  this->nodeType = TypesTable::getInstance()->getType(ERROR_TYPE_ID);
    return false;
  }

  virtual void print(ostream& os) {
    os << int(this) << "[label = \"Node\"]" << endl;
  }


  TypeExpression* getNodeType() {
	  if (!this->nodeType)
		  this->type_checking();
	  return this->nodeType;
  }
protected:
	TypeExpression* nodeType;

};
