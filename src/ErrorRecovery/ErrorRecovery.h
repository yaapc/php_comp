#ifndef ERR_H
#define ERR_H

#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

class ErrRecItem{
public:
	int lineNo;
	int colNo;
	char* txt;
	char* var;
	ErrRecItem * next;
};

class ErrRecQueue{
public:
	ErrRecItem * front;
	ErrRecItem * tail;
	void enqueue(int lineNo, int colNo, const char *txt, const char *varName)
	{
		ErrRecItem * item = new ErrRecItem();
		item->colNo = colNo;
		item->lineNo = lineNo;
		item->txt = new char[255];
		item->txt[0] = '\0';
		strcat(item->txt, txt);
		item->var = new char[255];
		item->var[0] = '\0';
		strcat(item->var, varName);
		if(front == 0){
			front = tail = item;
		}
		else{
			tail->next = item;
			tail = item;
		}
	}
	ErrRecItem * dequeue()
	{
		ErrRecItem * ret = 0;
		if(front != 0)
		{
			ret = front;
			if(front == tail)
			{
				front = tail = 0;
			}
			else
	     	{
			front = front->next;
		    }
		}

		return ret;
	}
	int isEmpty()
	{
		if(front == 0 )
			return 1;
		else
			return 0;
	}
};
class ErrorRecovery
{
	ofstream os;
public:
	ErrRecQueue * errQ;
	ErrRecQueue * warnQ;
	ErrorRecovery(void)
	{
		this->errQ = new ErrRecQueue();
		this->warnQ = new ErrRecQueue();
		os.open("errors.txt", fstream::out);
	}
	~ErrorRecovery(void){};
	void printErrQueue()
	{
		os << "----------------Parser Error----------------" << endl;
		if (errQ->isEmpty()){
			os << "No Error..." << endl;
		}else{
				while(!errQ->isEmpty()){
					ErrRecItem * e = errQ->dequeue();
					os<<"Error: ["<<e->lineNo<<","<<e->colNo<<"]   "<<e->txt<<",  "<<e->var<<"\n";
				}
		}

		os << "----------------Parser Warning--------------" << endl;

		if (warnQ->isEmpty()){
			os << "No Waring..." << endl;
		}else{
			while(!warnQ->isEmpty()){
				ErrRecItem * e = warnQ->dequeue();
				os<<"Warning: ["<<e->lineNo<<","<<e->colNo<<"]   "<<e->txt<<",  "<<e->var<<"\n";
			}
		}
		os << "--------------------------------------------" <<endl;
	}
};

#endif
