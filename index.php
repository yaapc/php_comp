<?php



  
class TEST{
    int $x = 10;
    function TEST(){}    
  }
  
    TEST $t = new TEST();
  function test (TEST $t) : void {
    echo $t->x;
  }
  


  test($t);
  


?>
