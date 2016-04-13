#pragma once
#ifndef TYPE_BOOLEAN_HPP
#define TYPE_BOOLEAN_HPP

#include "TypeExpression.hpp"
/*
 * A Singleton Class of Boolean Type.
 */
class TypeBoolean : public TypeExpression {

public:
    static TypeBoolean* getInstance(){
        if(!mInstance){
          mInstance = new TypeBoolean();
        }
        return mInstance;
    }

    int getTypeId(){
      return BOOLEAN_TYPE_ID;
    }

protected:
    static TypeBoolean* mInstance;
    TypeBoolean(){}

};

//static defifintion
TypeBoolean* TypeBoolean::mInstance;

#endif
