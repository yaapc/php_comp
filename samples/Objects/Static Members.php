<?php

  class A{
     static int     $x  = 50;
     static float   $y  = 6.45;
     static string  $z  = "1";
   
     public int $a;

     function A(int $paramter){
        $a = $paramter;
        $x += 10;
        $y *= 2.0;
        $z = $z + "@";
     }
	 
	public static function echoStatic() : void {
		echo "Static Members:";
		echo $x;
		echo $y;
		echo $z;
		
	}
  }

    A $a;
    int $i = 0;

    while($i <= 15)
    {
        $a = new A($i);
        $i+= 1;
    }

    echo A::$x;
    echo A::$y;
    echo A::$z;

	echo "================================";

	A::echoStatic();
    
?>