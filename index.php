<?php

//static int $sta;

const string HOST_IP = "127.0.0.1";
const int I_AM_CONST = 10;

$iAmUndecl;
//int[] $x = new int[100];



//TODO:
static int $iAmStatic;

//##16 variables should not be redeclared in the same scope
int $iAmInt,$iAmInt2,$iAmInt; 
float $iAmFloat = 1.5;

iAmClass $iAmClassObject = new iAmClass($arg);


function iAmAFunction(int $iAmIntParam, float $iAmFloatParam, int $iAmObjParam, int $iAmADefParm) : void {
	string $iAmString = "Hello World!";	
	$iAmIntParam = 0;	
	
	iAmFunction2();
}

function iAmFunction2() : string {
	bool $iAmBool;
}

function iAmFunction3(double $iAmDefault = 10) : void{
	
}

function iAmFunction4(int $param1,int $param2,double $param3) : int {
	
}

function iAmFunction5() : void {
	
}

final class iAmClass extends iAmBaseClass{
	
	public function iAmClass(){
		//Constructor
	}
	
	public static int $iAmStaticMember;
	public static const int iAmConstMember;
	
	
	protected static int $iAmMember;
	
	abstract public function methodMem2() : void ;
	
	public function getIAmMember() : int {
			char $iAmChar;
			
			$this->iAmMember2 = 10;
			
			for(int $i =0; $i < 10; $i++){
				int $iAmInForLoop = 9999;
				{
					int $innerInteger;
				}
			}
			
			for(int $i =0; $i< 15; $i++)
				int $iAmInASingleLoop;
			
			do {
				int $iAmInDoWhileLoop = 999;
			}while ($iAmChar = 0);
			
			if($iAmChar){
				int $iAmInASingleIf;	
			}
			
			if(true)
				int $iAmInASingleIf2;
			
			if($iAmChar = 'd'){
				int $iAmInAMultiIf;
			}else if($iAmChar = 'f'){
				int $iAmInAMultiIf2;
			}else 
				int $iAmInASingleElse;
			
			iAmBaseClass $iAmClass = new iAmBaseClass();

	}   

	abstract private function setIAmMember(int $param1, string $param2) : void;
	
	
	//##15  methods should not duplicated in the same class
	public function iAmDuplicated() : void {
		
	}
	public function iAmDuplicated() : void {
		
	}
	
	//error overriding final function from iAmBaseClass
	public function finalFunc(){
		
	}
	
	//base is public - sub is private == ERROR
	private function iAmPubFunc() : void {
		
	}
	
	//base is private == sub can't access it at all, NO ERROR every class has it's own
	public function iAmPrivFunc() : void {
		$this -> iAmPubFunc();
	}
	
	//base is protected - sub is private == ERROR
	private function iAmProtecFunc() : void {
		
	}
	
}


//##15 no duplicated class is allowed 
class iAmClass { 
	int $mem;
}

class iAmBaseClass {
	
	//##17 data members should be unique 
	private int $iAmInnerInt = 0;
	public int $iAmInnerInt = 1;
	
	private function iAmBaseClass() {
		
	}
	
	//##18 functions should not have similiar paramters
	function setIAm(int $param, int $param) : void {		
	}
	
	//##21 access modifier should not be assigned together 
	private public int $param;
	
	//syntax error
	priva public int $pa;
	
	public final function finalFunc() : void {
		
	}
	
	
	public function iAmPubFunc() : void {
		
	} 
	
	private function iAmPrivFunc() : void {
		
	}
	
	protected function iAmProtecFunc() : void {
		
	}
}


//##26 inheritance from undefined class is not allowed
class newClass extends iAmUndecClass {
	int $mem;
}

//TODO: ##28 variable names shouldn't start with a punctuation 
int $:param;
int $10param;


final class finalClass {
	int $param;
}

class finalInherit extends finalClass {
	int $param;
}

##33
final abstract class finalAbstClass {
	int $param;
}



while (true)
	int $iAmInAWHILE;


{
	
	int $iAmAlone;
		
}

class ClassA { 
	private int $a;
}

class ClassB { 
	private int $b;
}

class ClassC extends ClassE { 
	private int $c;
}

class ClassD extends ClassC{
	private int $d;
}

class ClassE extends ClassD{
	private int $e;
}


?>
