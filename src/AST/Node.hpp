#pragma once
#include <iostream>
#include "../Logger.hpp"
using namespace std;

static Logger astLog("ast_log.txt");

class Node {
public:

	virtual bool type_checking() {
		return false;
	}

	virtual void generate_code(){
		astLog.log("generate_code for Node (not implemented)");
	}

	virtual void print(ostream& os) {
	os << int(this) << "[label = \"Node\"]" << endl;
	}
};
