<?php
  class Parent
  {
    public int $x;
    public int $y;
  
    function Parent(){}

  
    function fun() : void {
      echo "I am Parent Method";
    }
    
    function fun2() : void {
      echo "I am also Parent Method";
    }
  }

class Child extends  Parent
{
    function Child(){}
    function fun() : void {
      echo "I am Child Method";
    }
}

  Parent $p;
  
  int $x = 10;
  
  if ($x == 10){
    $p = new Child();
  }else{
    $p = new Parent();
  }

  $p->fun();
  $p->fun2();

?>