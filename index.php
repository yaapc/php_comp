<?php

class TEST{
    int $x = 10;
    function TEST(){}    
  }

class TEST2 { 
    int $x = 20;  
  }

TEST2 $t = new TEST2();

function test (Object $t) : void {
    echo $t->to_string();
}
  

test($t);
  


?>
