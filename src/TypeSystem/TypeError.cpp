

#include "TypeError.hpp"


TypeError::TypeError(string msg):msg(msg){}

int TypeError::getTypeId()
	{
		return ERROR_TYPE_ID;
	}
