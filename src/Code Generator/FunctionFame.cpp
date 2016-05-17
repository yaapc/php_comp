#include "FunctionFrame.hpp"
#include "../SymbolTable/Symbol.h"
#include "AsmGenerator.h"

GlobalFrame::GlobalFrame()
{
	parentFrame				= nullptr;
	globalSize				= 0;
}

void GlobalFrame::addLocal(Node *node)
{
	DeclarationNode* declarationNode = dynamic_cast<DeclarationNode*>(node);
	Variable* variableSymbol = declarationNode->variable;


	//get the size of variable in Bytes
    int varSize  = node->getNodeType()->getSize();
	locals[declarationNode->variable->getNameWithout()] = globalSize;
	
	AsmGenerator::comment(declarationNode->variable->getNameWithout() + " in global scoop address "+to_string(globalSize)+" from gp");

	if (declarationNode->getNodeType()->getTypeId() == INTEGER_TYPE_ID 
			|| declarationNode->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID
			|| declarationNode->getNodeType()->getTypeId() == FLOAT_TYPE_ID){
		// int inital value = 0 , boolean inital value is false
		AsmGenerator::sw("0",to_string(globalSize)+"($gp)");
	}

	if (declarationNode->getNodeType()->getTypeId() == STRING_TYPE_ID){
		// String variables are allocated in heap but the address is stored in data
		// the address is stored as int with .word type 
		//string variable should be initialize to "" 
		//I stored "" in .data with a name empty_string and all string variables use it
		string s0 = "s0";
		// load the address of empty string in s0
		AsmGenerator::la(s0,AsmGenerator::empty_string_address);
		// store the address of empty string in variable address
		AsmGenerator::sw(s0,to_string(globalSize)+"($gp)");
		
	}

	if (declarationNode->getNodeType()->getTypeId() == CLASS_TYPE_ID){
		varSize = 4; // pointer to object 
		AsmGenerator::sw("0",to_string(globalSize)+"($gp)");
	}

	// Move Global Pointer
	globalSize += varSize;
}

string GlobalFrame::getAddress(string name)
{
	if (locals.find(name) != locals.end()) {
    	int offset = locals[name];
		return to_string(offset)+"($gp)";
	}else{
		cout << "This error should not be possible here, can not find symbol you're looking for." << endl;
	}
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
	paramtersOffset			= 0;
	stackSize				= 0;
	initialFrameSize		= 2*4;	// 4 for $fp and 4 for $ra
	this->parentFrame			= parent;
	for(auto &node : fdn->paramsList->nodes)
	{
		ParameterNode* paramterNode = dynamic_cast<ParameterNode*>(node);
		addParameter(paramterNode);
	}
}

void FunctionFrame::addParameter(ParameterNode *parameterNode)
{
	int parameterSize = parameterNode->getNodeType()->getSize();
	arguments[parameterNode->parSym->getNameWithout()] = paramtersOffset;
	paramtersOffset += parameterSize;
}

void FunctionFrame::addLocal(Node *node)
{
	DeclarationNode* variableDeclarationNode = dynamic_cast<DeclarationNode*>(node);
	int varSize = node->getNodeType()->getSize();
    stackSize += varSize;
	int variableOffset = -stackSize - initialFrameSize; // offset from frame pointer
	locals[variableDeclarationNode->variable->getNameWithout()] = variableOffset;
	AsmGenerator::comment(variableDeclarationNode->variable->getNameWithout() + " in function scoop address "+to_string(variableOffset)+" from fp");
}

string FunctionFrame::getAddress(string name)
{

	if (locals.find(name) != locals.end()) {
    	int offset = locals[name];
		return to_string(offset)+"($fp)";
    } 

	if (arguments.find(name) != arguments.end()) {
		int offset = arguments[name];
        return to_string(offset)+"($fp)";
    } 
		
	if (parentFrame) {
		return parentFrame->getAddress(name);
    }

	cout << "This error should not be possible here, can not find symbol you're looking for." << endl;
}

ObjectFrame::ObjectFrame()
{
	GlobalFrame();
	membersOffset = 0;
}

ObjectFrame::ObjectFrame(GlobalFrame *parent,ClassDefineNode *cdn)
{
	GlobalFrame();
	this->parentFrame			= parent;
	membersOffset = 0;

}

void ObjectFrame::addLocal(Node *node)
{
	ClassMemNode* classMemNode = dynamic_cast<ClassMemNode*>(node);
	Variable* variable = classMemNode->getMemSymbol();
	int classMemSize = classMemNode->getNodeType()->getSize();
	locals[variable->getNameWithout()] = membersOffset;
	membersOffset += classMemSize;
}

string ObjectFrame::getAddress(string name)
{

	if (locals.find(name) != locals.end()) {
		int offset = locals[name];
		return to_string(offset);
	} 

	if (functions.find(name) != functions.end()) {
		int offset = functions[name];
		return to_string(offset);
	}
	
}

void ObjectFrame::addFunction(Node *node)
{
	ClassMethodNode* classMethodNode = dynamic_cast<ClassMethodNode*>(node);
	Function* function = classMethodNode->methodSym;
	functions[function->getName()] = membersOffset;
	membersOffset += 4;
}
