<?php
  
  int $x = 15;
  float $y = 15.5;
  float $res;
  
	echo "Arithmatic Operations:";

	    $res = $x + $y;
	    echo $x," + ", $y , " = ", $res;

	    $res = $x - $y;
	    echo $x," - ", $y , " = ", $res;

	    $res = $x * $y;
	    echo $x," * ", $y , " = ", $res;

	    $res = $x / $y;
	    echo $x," / ", $y , " = ", $res,"\n ";
	
	echo "Relational Operations:";

	    if ($x == $y)
		    echo $x, " is (==) ",$y;
	    else
		    echo $x, " is not (==) ",$y;

	    if ($x != $y)
		    echo $x, " is (!=) ",$y;
	    else
		    echo $x, " is not (!=) ",$y;

	    if ($x > $y)
		    echo $x, " is (>) ",$y;
	    else
		    echo $x, " is not (>) ",$y;

	    if ($x >= $y)
		    echo $x, " is (>=) ",$y;
	    else
		    echo $x, " is not (>=) ",$y;

	    if ($x < $y)
		    echo $x, " is (<) ",$y;
	    else
		    echo $x, " is not (<) ",$y;

	    if ($x <= $y)
		    echo $x, " is (<=) ",$y;
	    else
		    echo $x, " is not (<=) ",$y;
  
  echo "\n";
  
  string $s = "String";
  
  string $sRes;
  
  $sRes = $s + $x;
    echo $sRes;
  
  $sRes = $x + $s;
    echo $sRes;
  
?>