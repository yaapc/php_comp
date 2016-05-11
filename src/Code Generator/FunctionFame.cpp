#include "FunctionFrame.hpp"
#include "../SymbolTable/Symbol.h"
#include "AsmGenerator.h"



void GlobalFrame::addVariable(DeclarationNode *declarationNode)
{
	Variable* variableSymbol = declarationNode->variable;

	if (declarationNode->getNodeType()->getTypeId() == INTEGER_TYPE_ID || declarationNode->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID){
		string variableAddress = variableSymbol->getUniqueName();
		AsmGenerator::store_global_int(variableAddress,0);
		AsmGenerator::comment("Store ("+variableAddress+") in data segment");
	}

	if (declarationNode->getNodeType()->getTypeId() == FLOAT_TYPE_ID){
		string variableAddress = variableSymbol->getUniqueName();
		AsmGenerator::store_global_float(variableAddress,0.0f);
		AsmGenerator::comment("Store ("+variableAddress+") in data segment");
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
		AsmGenerator::sw(s0,variableAddress);
		AsmGenerator::comment("Store ("+variableAddress+") in data segment");
	}
}

string GlobalFrame::getAddress(VariableNode *variableNode)
{
	string variableUniqeName = variableNode->variable->getUniqueName();
	AsmGenerator::comment("load ("+variableUniqeName + ") from data segment");
	//Global Variable address are only its unique name
	return variableUniqeName;

}

void GlobalFrame::addParameter(ParameterNode *parameterNode)
{
	throw std::invalid_argument("DAMN this shoudn't happen");
}

FunctionFrame::FunctionFrame()
{
	paramtersOffset			= 0;
	stackSize				= 0;
	initialFrameSize		= 2*4;	// 4 for $fp and 4 for $ra
}

FunctionFrame::FunctionFrame(FunctionFrame *parent,FunctionDefineNode *fdn)
{
	this->paramtersOffset		= 0;
	this->stackSize				= 0;
	this->initialFrameSize		= 2*4; // 4 for $fp and 4 for $ra
	this->parentFunctionFrame	= parent;
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

void FunctionFrame::addVariable(DeclarationNode *variableDeclarationNode)
{
	//ToDo get the size of variable in Bytes
    int varSize = 4;
    stackSize += varSize;
	int variableOffset = -stackSize - initialFrameSize; // offset from frame pointer
    locals[variableDeclarationNode->variable] = variableOffset;
	AsmGenerator::comment(variableDeclarationNode->variable->getUniqueName() + " in function scoop address "+to_string(variableOffset)+" from fp");
}

string FunctionFrame::getAddress(VariableNode *variableNode)
{
	if (locals.find(variableNode->variable) != locals.end()) {
    	int offset = locals[variableNode->variable];
		return to_string(offset)+"($fp)";
    } 

	if (arguments.find(variableNode->variable) != arguments.end()) {
		int offset = arguments[variableNode->variable];
        return to_string(offset)+"($fp)";
    } 
		
	if (parentFunctionFrame) {
		return parentFunctionFrame->getAddress(variableNode);
    }

	throw std::invalid_argument("This error should not be possible here, can not find symbol you're looking for.");
}

GlobalFrame::GlobalFrame():FunctionFrame(){}