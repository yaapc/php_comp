<?php
class XY
{
    int $x 				= 10;
    int $y 				= 20;
    string $className 	= "HeHe";
    
    function XY(){}
    
    function sum (int $i = 0): int {
      return $x + $y + $i;
    }
    
    function sub (): int {
      return $x - $y;
    }
    
    function mult (int $i = 1): int {
      return $x * $y * $i;
    }
    
    function div (): int {
      return $x / $y;
    }
    
    function to_string(): string {
		return $className +" $x $y";
    }
}


  XY $obj = new XY();

  $obj->x = 100;
  $obj->y = 50;
  $obj->className = "CLass XY";

  echo $obj->sum(10);
  echo $obj->sub();
  echo $obj->mult(5);
  echo $obj->div();
  echo $obj->to_string();
?>