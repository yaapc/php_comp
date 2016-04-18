#pragma once
#include <iostream>
#include "../Logger.hpp"
#include "../TypeSystem/TypesTable.h"

using namespace std;

static Logger astLog("ast_log.txt");

class Node {
public:

	virtual void generate_code(){
		astLog.log("generate_code Node (not implemented)");
	}

	virtual void print(ostream& os) {
	os << int(this) << "[label = \"Node\"]" << endl;
	}

  virtual bool type_checking() {
	  this->nodeType = TypesTable::getInstance()->getType(ERROR_TYPE_ID);
    return false;
  }


  TypeExpression* getNodeType() {
	  if (!this->nodeType)
		  this->type_checking();
	  return this->nodeType;
  }
protected:
	TypeExpression* nodeType;
};
