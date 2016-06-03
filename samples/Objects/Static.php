<?php

  class A{
     static int     $x  = 50;
     static float   $y  = 17.0;
     static string  $z  = "1";
   
     int $a;

     function A(int $paramter){
        $a = $paramter;
        $x += 10;
        $y *= 2.0;
        $z += $paramter;
     }

  }

    A $a;
    int $i = 0;

    for (int $i = 0 ; $i < 10 ; $i += 1){
        $a = new A($i);
    }

    echo A::$x;
    echo A::$y;
    echo A::$z;


    echo $a->a;
?>

