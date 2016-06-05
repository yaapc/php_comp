<?php
class XY
{
    public int $x 				= 10;
    public int $y 				= 20;
    public string $className 	= "HeHe";
    
    public function XY(){}
    
    public function sum (int $i = 0): int {
      return $x + $y + $i;
    }
    
    public function sub (): int {
      return $x - $y;
    }
    
    public function mult (int $i = 1): int {
      return $x * $y * $i;
    }
    
    public function div (): int {
      return $x / $y;
    }
    
    public function to_string(): string {
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