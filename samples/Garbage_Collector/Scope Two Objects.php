<?php
  
  class TEST{
    string $x1 = "String";
    function TEST(){}
    
  }
  TEST $t;
  
  if (true)
  {
    TEST $tt = new TEST();
    echo $tt->x1;
    $tt = $t;
    echo $tt->x1;
  }
 
?>