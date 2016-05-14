#include "FunctionFrame.hpp"
#include "../SymbolTable/Symbol.h"
#include "AsmGenerator.h"

GlobalFrame::GlobalFrame()
{
	parentFrame				= nullptr;
}

void GlobalFrame::addLocal(Node *node)
{
	DeclarationNode* declarationNode = dynamic_cast<DeclarationNode*>(node);

	Variable* variableSymbol = declarationNode->variable;

	if (declarationNode->getNodeType()->getTypeId() == INTEGER_TYPE_ID || declarationNode->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID){
		string variableAddress = variableSymbol->getUniqueName();
		AsmGenerator::comment("Store ("+variableAddress+") in data segment");
		AsmGenerator::store_global_int(variableAddress,0);
	}

	if (declarationNode->getNodeType()->getTypeId() == FLOAT_TYPE_ID){
		string variableAddress = variableSymbol->getUniqueName();
		AsmGenerator::comment("Store ("+variableAddress+") in data segment");
		AsmGenerator::store_global_float(variableAddress,0.0f);
	}

	if (declarationNode->getNodeType()->getTypeId() == STRING_TYPE_ID){
		// String variables are allocated in heap but the address is stored in data
		// the address is stored as int with .word type 
		string variableAddress = AsmGenerator::store_global_string(variableSymbol->getUniqueName(),NULL);

		//string variable should be initialize to "" 
		//I stored "" in .data with a name empty_string and all string variables use it

		string s0 = "s0";
		// load the address of empty string in s0
		AsmGenerator::la(s0,AsmGenerator::empty_string_address);
		// store the address of empty string in variable address
		AsmGenerator::comment("Store ("+variableAddress+") in data segment");
		AsmGenerator::sw(s0,variableAddress);
		
	}
}

string GlobalFrame::getAddress(Node *node)
{
	VariableNode* variableNode = dynamic_cast<VariableNode*>(node);
	string variableUniqeName = variableNode->variable->getUniqueName();
	AsmGenerator::comment("load ("+variableUniqeName + ") from data segment");
	//Global Variable address are only its unique name
	return variableUniqeName;

}

FunctionFrame::FunctionFrame()
{
	GlobalFrame();
	paramtersOffset			= 0;
	stackSize				= 0;
	initialFrameSize		= 2*4;	// 4 for $fp and 4 for $ra
}

FunctionFrame::FunctionFrame(GlobalFrame *parent,FunctionDefineNode *fdn)
{
	FunctionFrame();
	this->parentFrame			= parent;
	for(auto &node : fdn->paramsList->nodes)
	{
		ParameterNode* paramterNode = dynamic_cast<ParameterNode*>(node);
		addParameter(paramterNode);
	}
}

void FunctionFrame::addParameter(ParameterNode *parameterNode)
{
	//ToDo get the size of paramter in Bytes
	int parameterSize = 4;
	arguments[parameterNode->parSym] = paramtersOffset;
	paramtersOffset += parameterSize;
}

void FunctionFrame::addLocal(Node *node)
{
	DeclarationNode* variableDeclarationNode = dynamic_cast<DeclarationNode*>(node);
	//ToDo get the size of variable in Bytes
    int varSize = 4;
    stackSize += varSize;
	int variableOffset = -stackSize - initialFrameSize; // offset from frame pointer
    locals[variableDeclarationNode->variable] = variableOffset;
	AsmGenerator::comment(variableDeclarationNode->variable->getUniqueName() + " in function scoop address "+to_string(variableOffset)+" from fp");
}

string FunctionFrame::getAddress(Node *node)
{
	VariableNode* variableNode = dynamic_cast<VariableNode*>(node);
	if (locals.find(variableNode->variable) != locals.end()) {
    	int offset = locals[variableNode->variable];
		return to_string(offset)+"($fp)";
    } 

	if (arguments.find(variableNode->variable) != arguments.end()) {
		int offset = arguments[variableNode->variable];
        return to_string(offset)+"($fp)";
    } 
		
	if (parentFrame) {
		return parentFrame->getAddress(variableNode);
    }

	throw std::invalid_argument("This error should not be possible here, can not find symbol you're looking for.");
}

ObjectFrame::ObjectFrame()
{
	GlobalFrame();
	objectSize = 0;
}

ObjectFrame::ObjectFrame(GlobalFrame *parent,ClassDefineNode *cdn)
{
	GlobalFrame();
	this->parentFrame			= parent;
	objectSize = 0;

}

void ObjectFrame::addLocal(Node *node)
{
	ClassMemNode* classMemNode = dynamic_cast<ClassMemNode*>(node);
	Variable* variable = classMemNode->memberSym;
	//ToDo get the size of paramter in Bytes
	int classMemSize = 4;
	locals[variable] = classMemSize;
	objectSize += classMemSize;
}

string ObjectFrame::getAddress(Node *node)
{
	VariableNode* variableNode = dynamic_cast<VariableNode*>(node);
	if (locals.find(variableNode->variable) != locals.end()) {
    	int offset = locals[variableNode->variable];
		return to_string(offset)+"($fp)";
    } 

}
