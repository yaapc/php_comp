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
	}

	if (parentFrame){
		return parentFrame->getAddress(name);
	}
	cout << "This error should not be possible here, can not find symbol you're looking for." << endl;
	return nullptr;
}

FunctionFrame::FunctionFrame()
{
	GlobalFrame();
	paramtersOffset			= 0;
	stackSize				= 0;
	initialFrameSize		= 2*4;	// 4 for $fp and 4 for $ra
}

FunctionFrame::FunctionFrame(GlobalFrame *parent,ListNode *parametersNodes)
{
	paramtersOffset			= 0;
	stackSize				= 0;
	initialFrameSize		= 3*4;	// 4 for $fp and 4 for $ra and $a0
	this->parentFrame		= parent;
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
	stackSize += varSize;
	locals[variableDeclarationNode->variable->getNameWithout()] = -stackSize;
	AsmGenerator::comment(variableDeclarationNode->variable->getNameWithout() + " in function scoop address "+to_string(-stackSize - initialFrameSize)+" from fp");
}

string FunctionFrame::getAddress(string name)
{

	if (locals.find(name) != locals.end()) {
    	int offset = locals[name];
		return to_string(offset - initialFrameSize)+"($fp)";
    } 

	if (arguments.find(name) != arguments.end()) {
		int offset = arguments[name];
        return to_string((paramtersOffset - 4) - offset)+"($fp)";
    } 
		
	if (parentFrame) {
		return parentFrame->getAddress(name);
    }

	cout << "This error should not be possible here, can not find symbol you're looking for." << endl;
	return nullptr;
}

ObjectFrame::ObjectFrame()
{
	GlobalFrame();
	this->classType		= nullptr;
	this->parentFrame	= nullptr;
	objectsCount		= 0;
}

ObjectFrame::ObjectFrame(GlobalFrame *parent,TypeClass *classType)
{
	GlobalFrame();
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


void ObjectFrame::newObject()
{
	string s0 = "s0",s1 = "s1",s2 = "s2";

	// load the size of oject in $s0
	AsmGenerator::li(s0,classType->getSize());

	//syscall to allocate space for object, the address of new allocated memory $s1 (address of object)
	AsmGenerator::sbrk(s0,s1);

	// push the address into stack
	AsmGenerator::push(s1);

	AsmGenerator::comment("<Fill Functions Table");
	for(auto &memberWrapper :classType->getMembers())
	{
		if (memberWrapper->getWrapperType() == MemberWrapper::PROPERTY_WRAPPER)
		{
			PropertyWrapper* propertyWrapper = dynamic_cast<PropertyWrapper*>(memberWrapper);
		}

		if (memberWrapper->getWrapperType() == MemberWrapper::METHOD_WRAPPER){
			MethodWrapper* methodWrapper = dynamic_cast<MethodWrapper*>(memberWrapper);

			int methodOffset		= locals[methodWrapper->getName()];
			string methodLabel		= methodWrapper->getLabel();
			string methodAddress	= to_string(methodOffset)+"($" + s1 + ")"; // address in object

			AsmGenerator::la(s2,methodLabel);	//load the address of method
			AsmGenerator::sw(s2,methodAddress); //store the address of method in object
		}
	}
	AsmGenerator::comment("Fill Functions Table/>");
}


void ObjectFrame::fillFrame(TypeClass* typeClass)
{
	int memberOffset = 0;
	for(auto &memberWrapper :classType->getMembers())
	{
		if (memberWrapper->getWrapperType() == MemberWrapper::PROPERTY_WRAPPER)
		{
			PropertyWrapper* propertyWrapper = dynamic_cast<PropertyWrapper*>(memberWrapper);
			
			string propertyName		= propertyWrapper->getUniqueName(); // TODO replace getUniqueName with !!
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