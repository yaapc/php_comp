#ifndef ERR_H
#define ERR_H

#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

static int idCounter = 0;
class ErrRecItem{
public:
	int lineNo;
	int colNo;
	char* txt;
	char* var;
	ErrRecItem * next;
	int id;
};

class ErrRecQueue{
public:
	ErrRecItem * front;
	ErrRecItem * tail;
	int enqueue(int lineNo, int colNo, const char *txt, const char *varName)
	{
		ErrRecItem * item = new ErrRecItem();
		item->id = idCounter++;
		item->colNo = colNo;
		item->lineNo = lineNo;
		item->txt = new char[255];
		item->txt[0] = '\0';
		strcat(item->txt, txt);
		item->var = new char[255];
		item->var[0] = '\0';
		strcat(item->var, varName);
		if (front == 0){
			front = tail = item;
		}
		else{
			tail->next = item;
			tail = item;
		}
		return item->id;
	}
	ErrRecItem * dequeue()
	{
		ErrRecItem * ret = 0;
		if (front != 0)
		{
			ret = front;
			if (front == tail)
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
		if (front == 0)
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
		}
		else{
			while (!errQ->isEmpty()){
				ErrRecItem * e = errQ->dequeue();
				os << "Error: [" << e->lineNo << "," << e->colNo << "]   " << e->txt << ",  " << e->var << "\n";
			}
		}

		os << "----------------Parser Warning--------------" << endl;

		if (warnQ->isEmpty()){
			os << "No Waring..." << endl;
		}
		else{
			while (!warnQ->isEmpty()){
				ErrRecItem * e = warnQ->dequeue();
				os << "Warning: [" << e->lineNo << "," << e->colNo << "]   " << e->txt << ",  " << e->var << "\n";
			}
		}
		os << "--------------------------------------------" << endl;
	}

	void removeFromQueue(int id){
		ErrRecItem* qWalker = this->errQ->front;
		ErrRecItem* prev = qWalker;
		bool found = false;
		while (!found && qWalker != 0){
			if (qWalker->id == id){
				if (prev == qWalker){ // first item
					this->errQ->front = qWalker->next;
					found = true;
				}
				else if (qWalker->next == 0){
					prev->next = 0;
					this->errQ->tail = prev;
					found = true;
				}
				else{
					prev->next = qWalker->next;
					found = true;
				}
			}
			prev = qWalker;
			qWalker = qWalker->next;
		}
	}

};

#endif
