<?php

	float $x = 20.5;
	float $y = 20.5;
	float $res;

	echo "Initial value is ",$res,"\n ";

	echo "Arithmatic Float Operations:";

	$res = $x + $y;
	echo $x," + ", $y , " = ", $res;

	$res = $x - $y;
	echo $x," - ", $y , " = ", $res;

	$res = $x * $y;
	echo $x," * ", $y , " = ", $res;

	$res = $x / $y;
	echo $x," / ", $y , " = ", $res,"\n ";
	
	echo "Relational Float Operations:";

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
?>