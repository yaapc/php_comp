#include "Frame.hpp"
#include "../SymbolTable/Symbol.h"
#include "AsmGenerator.h"


GlobalFrame::GlobalFrame()
{
	parentFrame				= nullptr;
	frameSize				= 0;
	reg						= "($gp)";
}

void GlobalFrame::addLocal(Node *node)
{
	DeclarationNode* declarationNode = dynamic_cast<DeclarationNode*>(node);
	Variable* variableSymbol = declarationNode->variable;


	//get the size of variable in Bytes
    int varSize  = node->getNodeType()->getSize();
	locals[declarationNode->variable->getNameWithout()] = frameSize;
	
	AsmGenerator::comment(declarationNode->variable->getNameWithout() + " in global scoop address "+to_string(frameSize)+" from gp");

	if (declarationNode->getNodeType()->getTypeId() == INTEGER_TYPE_ID 
			|| declarationNode->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID
			|| declarationNode->getNodeType()->getTypeId() == FLOAT_TYPE_ID){
		// int inital value = 0 , boolean inital value is false
		AsmGenerator::sw("0",to_string(frameSize)+reg);
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
		AsmGenerator::sw(s0,to_string(frameSize)+reg);
		
	}

	if (declarationNode->getNodeType()->getTypeId() == CLASS_TYPE_ID){
		varSize = 4; // pointer to object 
		AsmGenerator::sw("0",to_string(frameSize)+reg);
	}

	// Move Global Pointer
	frameSize += varSize;
}

string GlobalFrame::getAddress(string name)
{
	if (locals.find(name) != locals.end()) {
    	int offset = locals[name];
		return to_string(offset)+reg;
	}

	if (parentFrame){
		return parentFrame->getAddress(name);
	}
	cout << "This error should not be possible here, can not find symbol you're looking for." << endl;
	return nullptr;
}

ScopeFrame::ScopeFrame(Frame *parent,bool isFunction)
{
	this->parentFrame		= parent;
	reg						= parent->reg;
	frameSize				= parent->frameSize;
	initialFrameSize		= parent->initialFrameSize;

	this->isFunction		= isFunction;
}

void ScopeFrame::addLocal(Node *node)
{
	if (!isFunction){
		DeclarationNode* declarationNode = dynamic_cast<DeclarationNode*>(node);
		Variable* variableSymbol = declarationNode->variable;


		//get the size of variable in Bytes
		int varSize  = node->getNodeType()->getSize();
		locals[declarationNode->variable->getNameWithout()] = frameSize;
	
		AsmGenerator::comment(declarationNode->variable->getNameWithout() + " in ??? scoop address "+to_string(frameSize)+" from "+reg);

		if (declarationNode->getNodeType()->getTypeId() == INTEGER_TYPE_ID 
				|| declarationNode->getNodeType()->getTypeId() == BOOLEAN_TYPE_ID
				|| declarationNode->getNodeType()->getTypeId() == FLOAT_TYPE_ID){
			// int inital value = 0 , boolean inital value is false
			AsmGenerator::sw("0",to_string(frameSize)+reg);
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
			AsmGenerator::sw(s0,to_string(frameSize)+reg);
			
			objectsLocals.push_back(declarationNode->variable->getNameWithout());
		}

		if (declarationNode->getNodeType()->getTypeId() == CLASS_TYPE_ID){
			varSize = 4; // pointer to object 
			AsmGenerator::sw("0",to_string(frameSize)+reg);
			//instead of add objects local here, i add it in assigment node so 
			//we can add also outer objects ref which being created with new operation in this scope
			//objectsLocals.push_back(declarationNode->variable->getNameWithout());
		}

		// Move Global Pointer
		frameSize += varSize;	
		return;
	}
	else{
		DeclarationNode* variableDeclarationNode = dynamic_cast<DeclarationNode*>(node);
		int varSize = node->getNodeType()->getSize();
		frameSize += varSize;
		locals[variableDeclarationNode->variable->getNameWithout()] = -frameSize;
		AsmGenerator::comment(variableDeclarationNode->variable->getNameWithout() + " in function scoop address "+to_string(-frameSize - initialFrameSize)+" from fp");
		return;
	}

}

string ScopeFrame::getAddress(string name)
{
	if (locals.find(name) != locals.end()) {
    	int offset = locals[name];

		if (!isFunction){
			return to_string(offset)+reg;
		}
		else{
			return to_string(offset - initialFrameSize)+reg;
		}
	}

	if (parentFrame){
		return parentFrame->getAddress(name);
	}
	cout << "This error should not be possible here, can not find symbol you're looking for." << endl;
	return nullptr;
}

FunctionFrame::FunctionFrame(Frame *parent,ListNode *parametersNodes)
{
	paramtersOffset			= 0;
	frameSize				= 0;
	initialFrameSize		= 3*4;	// 4 for $fp and 4 for $ra and $a0
	this->parentFrame		= parent;
	reg						= "($fp)";
	if (parametersNodes){
		for(auto &node : parametersNodes->nodes)
		{
			ParameterNode* paramterNode = dynamic_cast<ParameterNode*>(node);
			addParameter(paramterNode);
		}
	}
}

void FunctionFrame::addParameter(ParameterNode *parameterNode)
{
	int parameterSize =  4 ;// TODo parameterNode->getNodeType()->getSize();
	arguments[parameterNode->parSym->getNameWithout()] = paramtersOffset;
	paramtersOffset += parameterSize;
}

void FunctionFrame::addLocal(Node *node)
{
	DeclarationNode* variableDeclarationNode = dynamic_cast<DeclarationNode*>(node);
	int varSize = node->getNodeType()->getSize();
	frameSize += varSize;
	locals[variableDeclarationNode->variable->getNameWithout()] = -frameSize;
	AsmGenerator::comment(variableDeclarationNode->variable->getNameWithout() + " in function scoop address "+to_string(-frameSize - initialFrameSize)+" from fp");
}

string FunctionFrame::getAddress(string name)
{

	if (locals.find(name) != locals.end()) {
    	int offset = locals[name];
		return to_string(offset - initialFrameSize)+reg;
    } 

	if (arguments.find(name) != arguments.end()) {
		int offset = arguments[name];
        return to_string((paramtersOffset - 4) - offset)+reg;
    } 
		
	if (parentFrame) {
		return parentFrame->getAddress(name);
    }

	cout << "This error should not be possible here, can not find symbol you're looking for." << endl;
	return nullptr;
}

ObjectFrame::ObjectFrame(Frame *parent,TypeClass *classType)
{
	this->classType		= classType;
	this->parentFrame	= parent;
	objectsCount		= 0;
	classTagAddress		= AsmGenerator::store_string_literal(classType->getName());

	fillFrame(classType);
}

void ObjectFrame::addLocal(Node *node)
{

}

string ObjectFrame::getAddress(string name)
{
	if (locals.find(name) != locals.end()) {
		int offset = locals[name];

		if (thisReg.empty()){ // we are in function
			return to_string(offset) + "($a3)";
		}
		return to_string(offset) + "($" + thisReg + ")";
	} 

	if (parentFrame) {
		return parentFrame->getAddress(name);
    }

	cout << "This error should not be possible here, can not find symbol you're looking for." << endl;
	return nullptr;
}

void ObjectFrame::addFunction(Node *node)
{

}

void ObjectFrame::fillFrame(TypeClass* typeClass)
{
	int memberOffset = 0;
	for(auto &memberWrapper :classType->getMembers())
	{
		if (memberWrapper->getWrapperType() == MemberWrapper::PROPERTY_WRAPPER)
		{
			PropertyWrapper* propertyWrapper = dynamic_cast<PropertyWrapper*>(memberWrapper);
			
			string propertyName		= propertyWrapper->getNameWithout();
			int propertySize		= propertyWrapper->getSize();
			locals[propertyName]	= memberOffset;
			memberOffset			= memberOffset + propertySize;
		}

		if (memberWrapper->getWrapperType() == MemberWrapper::METHOD_WRAPPER)
		{
			MethodWrapper* methodWrapper = dynamic_cast<MethodWrapper*>(memberWrapper);

			string propertyName		= methodWrapper->getName();
			locals[propertyName]	= memberOffset;
			memberOffset			= memberOffset + 4;
		
		}
	}
}