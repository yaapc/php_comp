<?php
    int $x = 10;
    int $y = 115;
	int $res;
  
	echo "Initial value is ",$res,"\n ";
  
    echo "Arithmatic Integer Operations:";
  
    $res = $x + $y;
    echo "$x + $y = $res";
  
    $res = $x - $y;
    echo "$x - $y = $res";
  
    $res = $x * $y;
    echo "$x * $y = $res";
  
    $res = $x / $y;
    echo "$x / $y = $res";  
 
    $res = $x % $y;
    echo "$x % $y = $res\n ";
	
    echo "Relational Integer Operations:";

	if ($x == $y)
		echo "$x is (==) $y";
	else
		echo "$x is not (==) $y";

	if ($x != $y)
		echo "$x is (!=) $y";
	else
		echo "$x is not (!=) $y";
  
	if ($x > $y)
		echo "$x is (>) $y";
	else
		echo "$x is not (>) $y";
    
    if ($x >= $y)
		echo "$x is (>=) $y";
	else
		echo "$x is not (>=) $y";

	if ($x < $y)
		echo "$x is (<) $y";
	else
		echo "$x is not (<) $y";
    
    if ($x <= $y)
		echo "$x is (<=) $y";
	else
		echo "$x is not (<=) $y";    
?>