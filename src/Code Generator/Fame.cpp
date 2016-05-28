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
		
		}

		if (declarationNode->getNodeType()->getTypeId() == CLASS_TYPE_ID){
			varSize = 4; // pointer to object 
			AsmGenerator::sw("0",to_string(frameSize)+reg);
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
		parentFrame->frameSize += varSize;
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
		if (memberWrapper->getWrapperType() == MemberWrapper::PROPERTY_WRAPPER){
			PropertyWrapper* propertyWrapper	= dynamic_cast<PropertyWrapper*>(memberWrapper);
			DataMember* dataMember				= propertyWrapper->memberSymbol;
			int propertyOffset					= locals[dataMember->getNameWithout()];
			string propertyAddress				= to_string(propertyOffset)+"($" + s1 + ")"; // address in object
			if (dataMember->isInit()){

				if (propertyWrapper->getTypeExpr()->getTypeId() == INTEGER_TYPE_ID){
					AsmGenerator::li(s2,dataMember->getInitialValue().int_val);
					AsmGenerator::sw(s2,propertyAddress);
				}

				if (propertyWrapper->getTypeExpr()->getTypeId() == BOOLEAN_TYPE_ID){
					AsmGenerator::li(s2,dataMember->getInitialValue().bool_val);
					AsmGenerator::sw(s2,propertyAddress);
				}

				if (propertyWrapper->getTypeExpr()->getTypeId() == FLOAT_TYPE_ID){
					AsmGenerator::f_li("f0",dataMember->getInitialValue().float_val);
					AsmGenerator::ss("f0",propertyAddress);
				}

				if (propertyWrapper->getTypeExpr()->getTypeId() == STRING_TYPE_ID){
					string t0 = "t0",t1 = "t1";

					string stringAddress = AsmGenerator::store_string_literal(dataMember->getInitialValue().string_val);


					AsmGenerator::la(t0,stringAddress);
					
					AsmGenerator::move("a0",t0); // copy address of string literal into a0
					AsmGenerator::jal(AsmGenerator::strlen_functoion_name); // calculate length of string the result will be in $v1
					AsmGenerator::add_instruction("addi $t0,$v1,1"); // length++;
					AsmGenerator::sbrk(t0,s2);						// allcoate memory and store the address of newley crated memory in t0
					AsmGenerator::sw(s2,propertyAddress);

					AsmGenerator::la(t0,stringAddress);
					AsmGenerator::move("a0",s2); // put the address of newly created memory in a0 (the parameter of strcpy function)
					AsmGenerator::move("a1",t0); // put the address of string literal in a1 (the second parameter of strcpy function)
					AsmGenerator::jal(AsmGenerator::strcpy_function_name);
				}
			}else{
					
				if (propertyWrapper->getTypeExpr()->getTypeId() == STRING_TYPE_ID){
					// load the address of empty string in s0
					AsmGenerator::la(s2,AsmGenerator::empty_string_address);
					// store the address of empty string in variable address
					AsmGenerator::sw(s2,propertyAddress);
				}

			}
		}

		if (memberWrapper->getWrapperType() == MemberWrapper::METHOD_WRAPPER){
			MethodWrapper* methodWrapper	= dynamic_cast<MethodWrapper*>(memberWrapper);

			int methodOffset				= locals[methodWrapper->getName()];
			string methodLabel				= methodWrapper->getLabel();
			string methodAddress			= to_string(methodOffset)+"($" + s1 + ")"; // address in object

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