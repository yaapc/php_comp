#pragma once
#ifndef TYPESYSTEM_HPP
#define TYPESYSTEM_HPP

#include "TypeExpression.hpp"
#include <string>

/*
 * a Singleton class of TypesTable
   this class will represent the Type Factory which is responsible fro getting and creating
   Type Expressions as needed. 
 */

class TypesTable {
private:
	static TypesTable *mTypesTable;

public:
	static TypesTable* getInstance() {
		if (!mTypesTable) {
			mTypesTable = new TypesTable();
			return mTypesTable;
		}
		return mTypesTable;
	}


	/* this method will be the only way to access a specific type,
	 * method is responsible for locating the object associated with each type 
	 * expression and creating the necessary instance if the corresponding object has 
     * not been created before. Thus, the use of this method guarantees that no 
     * duplicate types are present, returning a pointer to the correct TypeExpression 
     * instance.
	*/
	TypeExpression* getType(string et) {

	}

};


#endif
