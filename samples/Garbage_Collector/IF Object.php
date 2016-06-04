<?php
  
  class TEST{
    string $x1 = "String";
    function TEST(){}
    
  }
  TEST $t;
  
  if (true)
  {
    $t = new TEST();
	echo $t->x1;
  }
  
  echo $t->x1;
 
?>