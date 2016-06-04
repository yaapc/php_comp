<?php
  
class TEST{
    string $x1 = "String 1";
	  string $x2 = "String 2";
    
    function TEST(){}
    
	  function cout() : void {
		  echo $x1," ",$x2;		
	  }
}

  while(true){
    TEST $t = new TEST();
    $t->cout();
  }
 
 
?>